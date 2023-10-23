/**
 * @file
 *  The following have been worked out:
 *  A. BTB indexed with PC
 *  B. BTB indexed with hash of PC and global path history
 *
 *  Some assumptions are: History is less than 16b
 *  Value counts is less than 16b
 */

#include <iostream>
#include <iomanip>
#include <memory>
#include <fstream>
#include <vector>
using std::cerr;
using std::endl;
using std::string;

#define BILLION 1'000'000'000
typedef unsigned int CADDRINT;

std::unique_ptr<std::ostream> out;
std::unique_ptr<std::ostream> logOut;

struct Target
{
    // TargetHolder()
    CADDRINT target;
    CADDRINT tag;
    char flags; // have bit 2 as valid, bit 1-0 as LRU
    bool target_valid_bit()
    {
        return (flags & (1 << 3));
    }
    void target_valid_bit_on()
    {
        flags |= (1 << 3);
    }
    void target_valid_bit_off()
    {
        flags &= ~(1 << 3);
    }
};

struct TargetHolder
{
    Target target;
    TargetHolder *next;
};

class Cache
{
private:
    int sets;
    int ways;
    int set_index_bits;
    Target invalid_target;
    std::vector<TargetHolder *> cache;
    CADDRINT get_tag(const CADDRINT addr)
    {
        int bit_handler = ((1 << set_index_bits) - 1);
        return addr & ~bit_handler;
    }
    unsigned int get_set_index(const CADDRINT addr)
    {
        int bit_handler = ((1 << set_index_bits) - 1);
        return addr & bit_handler;
    }

public:
    Cache(int set_index_bits, int ways) : sets(1 << set_index_bits), ways(ways), cache(1 << set_index_bits)
    {
        this->set_index_bits = set_index_bits;
        for (int idx = 0; idx < this->sets; idx++)
        {
            cache[idx] = NULL;
            for (int way = 0; way < this->ways; way++)
            {
                TargetHolder *th = new TargetHolder();
                th->target.target_valid_bit_off();
                th->next = cache[idx];
                cache[idx] = th;
            } // each idx should have "ways" number of TargetHolders
        }
        invalid_target = Target();
        invalid_target.target_valid_bit_off();
    }

    void show_target(Target *tg)
    {
        std::cout << tg->target_valid_bit() << " " << tg->tag << " " << tg->target << endl;
    }
    void show_index(const CADDRINT addr)
    {
        std::cout << "addr: " << addr << endl;
        int idx = get_set_index(addr);
        std::cout << "set_index: " << idx << std::endl;
        TargetHolder *th = this->cache[idx];
        while (th)
        {
            Target tg = th->target;
            show_target(&tg);
            th = th->next;
        }
    }
    // if target found in one of the four ways
    // this updates LRU positioning and returns the target
    Target get_target_at_idx_tag(int idx, CADDRINT tag)
    {
        TargetHolder *slowerTh = NULL;
        TargetHolder *th = this->cache[idx];
        bool found = false;
        for (int i = 0; i < this->ways; i++)
        {
            if (!th->target.target_valid_bit())
                continue;
            // check tag
            if (th->target.tag == tag)
            {
                found = true;
                break;
            }
            // next
            slowerTh = th;
            th = th->next;
        }
        // if th iterates through all, then th == NULL, and not found
        if (!found)
        {
            return invalid_target;
        }
        // if found,
        // already at head of LL
        if (slowerTh == NULL)
            return th->target;
        // shift th to head of LL
        slowerTh->next = th->next;
        th->next = this->cache[idx];
        this->cache[idx] = th;
        return th->target;
    }
    // call update only when tag didn't match!
    // modifies the least recently used TargetHolder
    // as the current addr and target
    // and makes it most recently used
    void update_target_at_idx_tag(int idx, CADDRINT tag, CADDRINT target)
    {
        TargetHolder *slowerTh = NULL;
        TargetHolder *th = this->cache[idx];
        for (int i = 0; i < this->ways - 1; i++)
        {
            slowerTh = th;
            th = th->next;
        }
        // th reaches the last node
        Target *tg = &(th->target);
        tg->target_valid_bit_on();
        tg->tag = tag;
        tg->target = target;

        // now place this node at front
        slowerTh->next = th->next;
        th->next = this->cache[idx];
        this->cache[idx] = th;
    }

    Target get_target(CADDRINT addr)
    {
        int idx = get_set_index(addr);
        CADDRINT tag = get_tag(addr);
        return get_target_at_idx_tag(idx, tag);
    }

    Target get_target_pro(CADDRINT addr, CADDRINT hist)
    {
        int addr_idx = get_set_index(addr);
        int hist_idx = get_set_index(hist);
        int idx = addr_idx ^ hist_idx;
        CADDRINT tag = addr;
        return get_target_at_idx_tag(idx, tag);
    }

    void update_target(CADDRINT addr, CADDRINT target)
    {
        int idx = get_set_index(addr);
        CADDRINT tag = get_tag(addr);
        update_target_at_idx_tag(idx, tag, target);
    }

    void update_target_pro(CADDRINT addr, CADDRINT hist, CADDRINT target)
    {
        int addr_idx = get_set_index(addr);
        int hist_idx = get_set_index(hist);
        int idx = addr_idx ^ hist_idx;
        CADDRINT tag = addr;
        update_target_at_idx_tag(idx, tag, target);
    }
};

class BTB
{
private:
    unsigned int correct_predictions;
    unsigned int missed_predictions;
    CADDRINT get_input_to_cache(CADDRINT addr)
    {
        return addr;
    }

public:
    std::unique_ptr<Cache> cache_block;
    BTB()
    {
        cache_block = std::make_unique<Cache>(7, 4);
        correct_predictions = 0;
        missed_predictions = 0;
    }

    void predict(CADDRINT pc, CADDRINT next_pc, CADDRINT target)
    {
        CADDRINT input = get_input_to_cache(pc);
        Target output = cache_block->get_target(input);
        bool isValid = output.target_valid_bit();
        if (!isValid)
        {
            missed_predictions++;
            if (target == next_pc)
                correct_predictions++;
        }
        if (isValid && (target == output.target))
            correct_predictions++;
        else
        {
            cache_block->update_target(input, target);
        }
    }
    unsigned int show_correct_predictions()
    {
        return correct_predictions;
    }
    unsigned int show_missed_predictions()
    {
        return missed_predictions;
    }
};

class BTB_pro
{
private:
    unsigned int correct_predictions;
    unsigned int missed_predictions;
    CADDRINT get_input_to_cache(CADDRINT addr)
    {
        return addr;
    }

public:
    std::unique_ptr<Cache> cache_block;
    BTB_pro()
    {
        cache_block = std::make_unique<Cache>(7, 4);
        correct_predictions = 0;
        missed_predictions = 0;
    }

    void predict(CADDRINT pc, CADDRINT next_pc, CADDRINT target, CADDRINT hist)
    {
        CADDRINT input = get_input_to_cache(pc);
        Target output = cache_block->get_target_pro(input, hist);
        bool isValid = output.target_valid_bit();
        if (!isValid)
        {
            missed_predictions++;
            if (target == next_pc)
                correct_predictions++;
        }
        if (isValid && (target == output.target))
            correct_predictions++;
        else
        {
            cache_block->update_target_pro(input, hist, target);
        }
    }
    unsigned int show_correct_predictions()
    {
        return correct_predictions;
    }
    unsigned int show_missed_predictions()
    {
        return missed_predictions;
    }
};
