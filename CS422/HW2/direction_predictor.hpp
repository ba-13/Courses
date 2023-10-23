/*
 * SPDX-License-Identifier: MIT
 */

/**
 * @file
 *  The following have been worked out:
 *  A. Static forward not-taken and backward taken (FNBT)
 *  B. Bimodal
 *  C. SAg
 *  D. GAg
 *  E. gshare
 *  F. Hybrid of SAg and GAg
 *  G. Hybrid of SAg, GAg, and gshare
 *
 *  Some assumptions are: History is less than 16b
 *  Value counts is less than 16b
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#define BILLION 1'000'000'000
typedef unsigned int CADDRINT;

using std::cerr;
using std::endl;
using std::string;
std::unique_ptr<std::ostream> out;
std::unique_ptr<std::ostream> logOut;
int working;

class DirectionPredictor
{
protected:
    bool prediction_taken;
    unsigned int correct_predictions;
    unsigned int fcorrect_predictions;
    unsigned int bcorrect_predictions;

public:
    std::string dp_name;
    virtual void show_status() = 0;
    virtual void predict(CADDRINT pc, CADDRINT target) = 0;
    virtual void update(bool actual_taken, bool is_forward) = 0;
    // to check what the last prediction of the dp was
    bool show_prediction()
    {
        return prediction_taken;
    }
    unsigned int show_correct_predictions()
    {
        return this->correct_predictions;
    }
    unsigned int show_fcorrect_predictions()
    {
        return this->fcorrect_predictions;
    }
    unsigned int show_bcorrect_predictions()
    {
        return this->bcorrect_predictions;
    }
};

class BHT
{
protected:
    int rows; // number of types of histories
    int cols; // history bits
    std::vector<UINT16> bht_table;

    int hash_to_bht(CADDRINT addr)
    {
        return (addr % this->rows);
    }
    UINT16 bht_value_history(int idx)
    {
        return this->bht_table[idx];
    }

public:
    BHT(int rows, int cols) : rows(rows), cols(cols), bht_table(rows, 0) { ; };

    void history_update(CADDRINT addr, bool actual_taken)
    {
        int actual_taken_int = actual_taken ? 1 : 0;
        int idx = BHT::hash_to_bht(addr);
        bht_table[idx] = (bht_table[idx] << 1) + actual_taken_int;
    }
    UINT16 addr_to_history(CADDRINT addr)
    {
        return BHT::bht_value_history(BHT::hash_to_bht(addr));
    }
    // used to print status of bht
    void show_status()
    {
        *logOut << "BHT details:" << endl;
        *logOut << "Still zero elements in indices:" << endl;
        for (unsigned int i = 0; i < this->bht_table.size(); i++)
        {
            if (this->bht_table[i] == 0)
                *logOut << i << " ";
        }
        *logOut << endl;
    }
};

class PHT_Norm
{
protected:
    unsigned int rows; // assuming rows is exponent of 2
    unsigned int cols; // number of bits per value
    std::vector<UINT16> pht_table;

    int hash_to_pht(UINT16 history)
    {
        return (history & (this->rows - 1));
    }
    bool pht_value_taken(int idx)
    {
        return (pht_table[idx] >= (1 << (cols - 1)));
    }

public:
    PHT_Norm(int rows, int cols) : rows(rows), cols(cols), pht_table(rows, 0) { ; };

    bool history_to_predict(UINT16 history)
    {
        return PHT_Norm::pht_value_taken(PHT_Norm::hash_to_pht(history));
    }
    void value_update(UINT16 history, bool actual_taken)
    {
        int idx = PHT_Norm::hash_to_pht(history);
        if (actual_taken)
            this->pht_table[idx] = std::min(this->pht_table[idx] + 1, (1 << cols) - 1);
        else
            this->pht_table[idx] = std::max(this->pht_table[idx] - 1, 0);
    }
    // used to print status of pht
    void show_status()
    {
        *logOut << "PHT_Norm details:" << endl;
        *logOut << "Still zero elements in indices:" << endl;
        for (unsigned int i = 0; i < this->pht_table.size(); i++)
        {
            if (this->pht_table[i] == 0)
                *logOut << i << " ";
        }
        *logOut << endl;
    }
};

class PHT_Xor
{
protected:
    int rows; // assuming rows is exponent of 2
    int cols; // number of bits per value
    std::vector<UINT16> pht_table;

    int hash_to_pht(UINT16 history, CADDRINT pc)
    {
        return ((history ^ pc) & (this->rows - 1));
    }
    bool pht_value_taken(int idx)
    {
        return (this->pht_table[idx] >= (1 << (this->cols - 1)));
    }

public:
    CADDRINT pc;
    PHT_Xor(int rows, int cols) : rows(rows), cols(cols), pht_table(rows, 0) { ; };

    bool history_to_predict(UINT16 history, CADDRINT pc)
    {
        this->pc = pc;
        return PHT_Xor::pht_value_taken(PHT_Xor::hash_to_pht(history, pc));
    }
    void value_update(UINT16 history, bool actual_taken)
    {
        int idx = PHT_Xor::hash_to_pht(history, this->pc);
        if (actual_taken)
            this->pht_table[idx] = std::min(this->pht_table[idx] + 1, (1 << cols) - 1);
        else
            this->pht_table[idx] = std::max(this->pht_table[idx] - 1, 0);
    }
    // used to print status of pht
    void show_status()
    {
        *logOut << "PHT_Xor details:" << endl;
        *logOut << "Still zero elements in indices:" << endl;
        for (unsigned int i = 0; i < this->pht_table.size(); i++)
        {
            if (this->pht_table[i] == 0)
                *logOut << i << " ";
        }
        *logOut << endl;
    }
};

class FNBT : public DirectionPredictor
{
public:
    FNBT()
    {
        dp_name = "FNBT";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
    }
    void show_status() override { ; };
    void predict(CADDRINT pc, CADDRINT target) override
    {
        if (pc > target)
            prediction_taken = true;
        else
            prediction_taken = false;
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
    }
};

class Bimodal : public DirectionPredictor
{
private:
    CADDRINT last_pc;
    std::unique_ptr<PHT_Norm> pht; // 512 x 2
    static const int PHT_ROWS = 512;
    static const int PHT_BITS = 2;

public:
    Bimodal()
    {
        dp_name = "Bimodal";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        last_pc = 0; // NULL
        pht = std::make_unique<PHT_Norm>(PHT_ROWS, PHT_BITS);
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        pht->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        prediction_taken = pht->history_to_predict(pc);
        last_pc = pc;
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        pht->value_update(last_pc, actual_taken);
    }
};

class SAg : public DirectionPredictor
{
private:
    CADDRINT last_pc;
    std::unique_ptr<PHT_Norm> pht;
    std::unique_ptr<BHT> bht;
    static const int BHT_ROWS = 1024;
    static const int BHT_HISTORY_BITS = 9;
    static const int PHT_ROWS = (1 << BHT_HISTORY_BITS);
    static const int PHT_BITS = 2;

public:
    SAg()
    {
        dp_name = "SAg";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        last_pc = 0; // NULL
        bht = std::make_unique<BHT>(BHT_ROWS, BHT_HISTORY_BITS);
        pht = std::make_unique<PHT_Norm>(PHT_ROWS, PHT_BITS);
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        pht->show_status();
        bht->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        UINT16 history = bht->addr_to_history(pc);
        prediction_taken = pht->history_to_predict(history);
        last_pc = pc;
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        // first update same PHT
        UINT16 curr_history = bht->addr_to_history(last_pc);
        pht->value_update(curr_history, actual_taken);
        // now update BHT
        bht->history_update(last_pc, actual_taken);
    }
};

class GAg : public DirectionPredictor
{
private:
    CADDRINT last_pc;
    std::unique_ptr<PHT_Norm> pht;
    std::unique_ptr<BHT> bht;
    static const int BHT_ROWS = 1;
    static const int BHT_HISTORY_BITS = 9;
    static const int PHT_ROWS = (1 << BHT_HISTORY_BITS);
    static const int PHT_BITS = 2;

public:
    GAg()
    {
        dp_name = "GAg";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        last_pc = 0; // NULL
        bht = std::make_unique<BHT>(BHT_ROWS, BHT_HISTORY_BITS);
        pht = std::make_unique<PHT_Norm>(PHT_ROWS, PHT_BITS);
    }

    // only lower BHT_HISTORY_BITS are valid, rest is 0
    UINT16 get_ghr()
    {
        return (this->bht->addr_to_history(0) & (PHT_ROWS - 1));
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        pht->show_status();
        bht->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        UINT16 history = bht->addr_to_history(pc);
        prediction_taken = pht->history_to_predict(history);
        last_pc = pc;
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        // first update same PHT
        UINT16 curr_history = bht->addr_to_history(last_pc);
        pht->value_update(curr_history, actual_taken);
        // now update BHT
        bht->history_update(last_pc, actual_taken);
    }
};

class Gshare : public DirectionPredictor
{
private:
    CADDRINT last_pc;
    std::unique_ptr<PHT_Xor> pht;
    std::unique_ptr<BHT> bht;
    static const int BHT_ROWS = 1;
    static const int BHT_HISTORY_BITS = 9;
    static const int PHT_ROWS = (1 << BHT_HISTORY_BITS);
    static const int PHT_BITS = 3;

public:
    Gshare()
    {
        dp_name = "gshare";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        last_pc = 0; // NULL
        bht = std::make_unique<BHT>(BHT_ROWS, BHT_HISTORY_BITS);
        pht = std::make_unique<PHT_Xor>(PHT_ROWS, PHT_BITS);
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        pht->show_status();
        bht->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        pht->pc = pc;
        UINT16 history = bht->addr_to_history(pc);
        prediction_taken = pht->history_to_predict(history, pc);
        last_pc = pc;
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        // first update same PHT
        UINT16 curr_history = bht->addr_to_history(last_pc);
        pht->value_update(curr_history, actual_taken);
        // now update BHT
        bht->history_update(last_pc, actual_taken);
    }
};

class T_SAgGAg : public DirectionPredictor
{
private:
    // Meta Predictor if gives true, then choose SAg, else GAg
    std::unique_ptr<PHT_Norm> meta_sag_gag;
    std::unique_ptr<SAg> sag;
    std::unique_ptr<GAg> gag;
    static const int META_ROWS = 512;
    static const int META_BITS = 2;

public:
    T_SAgGAg()
    {
        dp_name = "T_SAgGAg";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        sag = std::make_unique<SAg>();
        gag = std::make_unique<GAg>();
        meta_sag_gag = std::make_unique<PHT_Norm>(META_ROWS, META_BITS);
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        meta_sag_gag->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        // make predictions from individual DPs
        sag->predict(pc, target);
        gag->predict(pc, target);
        // index into meta_sag_gag
        UINT16 ghr = gag->get_ghr();
        bool meta_pred = meta_sag_gag->history_to_predict(ghr);
        prediction_taken = meta_pred ? sag->show_prediction() : gag->show_prediction();
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        // save the original ghr
        UINT16 ghr = gag->get_ghr();
        bool sag_pred = sag->show_prediction();
        bool gag_pred = gag->show_prediction();
        // update SAg
        sag->update(actual_taken, is_forward);
        // update GAg (and it's ghr)
        gag->update(actual_taken, is_forward);
        // update meta predictor
        if (sag_pred != gag_pred)
        {
            if (sag_pred == actual_taken)
                meta_sag_gag->value_update(ghr, true);
            else
                meta_sag_gag->value_update(ghr, false);
        }
    }
};

class M_SAgGAgGshare : public DirectionPredictor
{
private:
    std::unique_ptr<SAg> sag;
    std::unique_ptr<GAg> gag;
    std::unique_ptr<Gshare> gshare;

public:
    M_SAgGAgGshare()
    {
        dp_name = "M_SAgGAgGshare";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        sag = std::make_unique<SAg>();
        gag = std::make_unique<GAg>();
        gshare = std::make_unique<Gshare>();
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        gag->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        sag->predict(pc, target);
        gag->predict(pc, target);
        gshare->predict(pc, target);
        bool sag_pred = sag->show_prediction();
        bool gag_pred = gag->show_prediction();
        bool gshare_pred = gshare->show_prediction();
        prediction_taken = (sag_pred + gag_pred + gshare_pred) > 1 ? true : false;
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        sag->update(actual_taken, is_forward);
        gag->update(actual_taken, is_forward);
        gshare->update(actual_taken, is_forward);
    }
};

class T_SAgGAgGshare : public DirectionPredictor
{
private:
    std::unique_ptr<SAg> sag;
    std::unique_ptr<GAg> gag;
    std::unique_ptr<Gshare> gshare;
    static const int META_ROWS = 512;
    static const int META_BITS = 2;
    std::unique_ptr<PHT_Norm> meta_sag_gag;
    std::unique_ptr<PHT_Norm> meta_gag_gshare;
    std::unique_ptr<PHT_Norm> meta_gshare_sag;

public:
    T_SAgGAgGshare()
    {
        dp_name = "T_SAgGAgGshare";
        prediction_taken = false;
        correct_predictions = 0;
        fcorrect_predictions = 0;
        bcorrect_predictions = 0;
        sag = std::make_unique<SAg>();
        gag = std::make_unique<GAg>();
        gshare = std::make_unique<Gshare>();
        meta_sag_gag = std::make_unique<PHT_Norm>(META_ROWS, META_BITS);
        meta_gag_gshare = std::make_unique<PHT_Norm>(META_ROWS, META_BITS);
        meta_gshare_sag = std::make_unique<PHT_Norm>(META_ROWS, META_BITS);
    }

    void show_status() override
    {
        *logOut << "===============================================" << endl;
        *logOut << dp_name << " status:\n";
        gag->show_status();
        *logOut << "===============================================" << endl;
    }
    void predict(CADDRINT pc, CADDRINT target) override
    {
        sag->predict(pc, target);
        gag->predict(pc, target);
        gshare->predict(pc, target);
        bool sag_pred = sag->show_prediction();
        bool gag_pred = gag->show_prediction();
        bool gshare_pred = gshare->show_prediction();
        UINT16 ghr = gag->get_ghr();

        bool meta_pred;
        meta_pred = meta_sag_gag->history_to_predict(ghr);
        prediction_taken = meta_pred ? sag_pred : gag_pred;
        if (meta_pred) // SAg won
        {
            meta_pred = meta_gshare_sag->history_to_predict(ghr);
            prediction_taken = meta_pred ? gshare_pred : prediction_taken;
        }
        else // GAg won
        {
            meta_pred = meta_gag_gshare->history_to_predict(ghr);
            prediction_taken = meta_pred ? prediction_taken : gshare_pred;
        }
    }
    void update(bool actual_taken, bool is_forward) override
    {
        if (actual_taken == prediction_taken)
        {
            correct_predictions++;
            fcorrect_predictions += is_forward;
            bcorrect_predictions += !is_forward;
        }
        UINT16 ghr = gag->get_ghr();
        bool sag_pred = sag->show_prediction();
        bool gag_pred = gag->show_prediction();
        bool gshare_pred = gshare->show_prediction();
        // update SAg
        sag->update(actual_taken, is_forward);
        // update GAg (and it's ghr)
        gag->update(actual_taken, is_forward);
        // update Gshare
        gshare->update(actual_taken, is_forward);
        // Meta table updates
        if (sag_pred != gag_pred)
        {
            if (sag_pred == actual_taken)
                meta_sag_gag->value_update(ghr, true);
            else
                meta_sag_gag->value_update(ghr, false);
        }
        if (gag_pred != gshare_pred)
        {
            if (gag_pred == actual_taken)
                meta_gag_gshare->value_update(ghr, true);
            else
                meta_gag_gshare->value_update(ghr, false);
        }
        if (gshare_pred != sag_pred)
        {
            if (gshare_pred == actual_taken)
                meta_gshare_sag->value_update(ghr, true);
            else
                meta_gshare_sag->value_update(ghr, false);
        }
    }
};
