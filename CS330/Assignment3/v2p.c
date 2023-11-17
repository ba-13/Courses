#include <types.h>
#include <mmap.h>
#include <fork.h>
#include <v2p.h>
#include <memory.h>
#include <page.h>

/*
 * You may define macros and other helper functions here
 * You must not declare and use any static/global variables
 * */

#define FOURKB 4096

#define DEBUG 0 // Set to 1 to enable debugging, 0 to disable

#if DEBUG
#define debug_printk(fmt, ...) printk(fmt, ##__VA_ARGS__)
#else
#define debug_printk(fmt, ...) ((void)0)
#endif

static inline void invlpg(unsigned long addr)
{
    asm volatile("invlpg (%0)" ::"r"(addr) : "memory");
}

u64 PGD_IDX(u64 addr)
{
    return (addr >> (9 + 9 + 9 + 12)) & ((1 << 9) - 1);
}
u64 PUD_IDX(u64 addr)
{
    return (addr >> (9 + 9 + 12)) & ((1 << 9) - 1);
}
u64 PMD_IDX(u64 addr)
{
    return (addr >> (9 + 12)) & ((1 << 9) - 1);
}
u64 PTE_IDX(u64 addr)
{
    return (addr >> (12)) & ((1 << 9) - 1);
}

u64 page_aligned_start_addr(u64 addr)
{
    return (addr * FOURKB) / FOURKB;
}

u64 page_aligned_end_addr(u64 addr)
{
    u64 num_pages = addr / FOURKB;
    if (addr % FOURKB)
        num_pages += 1;
    return num_pages * FOURKB;
}

struct vm_area *make_new_vma(unsigned long start, unsigned long end, struct vm_area *next, int prot)
{
    stats->num_vm_area++;
    struct vm_area *new_vma = os_alloc(sizeof(struct vm_area));
    new_vma->access_flags = prot;
    new_vma->vm_start = start;
    new_vma->vm_end = end;
    new_vma->vm_next = next;
    debug_printk("[OS] [new_vma] Creating %x vma from %x to %x\n", prot, start, end);
    return new_vma;
}

void remove_vma(struct vm_area *vma)
{
    stats->num_vm_area--;
    os_free((void *)vma, sizeof(struct vm_area));
}

char merge_front(struct vm_area *front, struct vm_area *back)
{
    if (back == NULL || front == NULL)
        return 0;
    if (front->access_flags != back->access_flags)
        return 0;
    if (front->vm_end != back->vm_start)
        return 0;
    front->vm_end = back->vm_end;
    front->vm_next = back->vm_next;
    remove_vma(back);
    return 1;
}

/**
 * Change permissions physical memory
 */
void change_permissions_page(struct exec_context *current, u64 addr, int prot)
{
    addr = page_aligned_start_addr(addr);
    prot = (prot >> 1) << 3;
    u64 pgd_idx = PGD_IDX(addr);
    u64 pud_idx = PUD_IDX(addr);
    u64 pmd_idx = PMD_IDX(addr);
    u64 pte_idx = PTE_IDX(addr);
    char present = 1;
    u64 *pgd = osmap(current->pgd);
    present = present && (pgd[pgd_idx] & 0x1); // pud present
    if (!present)
        return;
    else
        pgd[pgd_idx] |= prot;
    u64 *pud = osmap(pgd[pgd_idx] >> 12);
    present = present && (pud[pud_idx] & 0x1); // pmd present
    if (!present)
        return;
    else
        pud[pud_idx] |= prot;
    u64 *pmd = osmap(pud[pud_idx] >> 12);
    present = present && (pmd[pmd_idx] & 0x1); // pte present
    if (!present)
        return;
    else
        pmd[pmd_idx] |= prot;
    u64 *pte = osmap(pmd[pmd_idx] >> 12);      // page itself
    present = present && (pte[pte_idx] & 0x1); // page present
    if (!present)
        return;
    debug_printk("[OS] [protect] page addr: %x, addr: %x, prot: %x\n", (u64 *)osmap(pte[pte_idx] >> 12), addr, prot);
    if (get_pfn_refcount(pte[pte_idx] >> 12) == 1) //* if this is above 1, then shared page frame
        pte[pte_idx] = (pte[pte_idx] & ~(1 << 3)) | prot;
    invlpg(addr); // removing page from TLB
}

/**
 ** mprotect System call Implementation.
 */
long vm_area_mprotect(struct exec_context *current, u64 addr, int length, int prot)
{
    if ((prot != PROT_READ) && (prot != (PROT_READ | PROT_WRITE)))
        return -EINVAL;
    u64 p1 = page_aligned_start_addr(addr);
    u64 p2 = page_aligned_end_addr(addr + length);
    debug_printk("[OS] [protect] Called for %x to %x\n", p1, p2);
    struct vm_area *prev = current->vm_area;
    if (prev == NULL)
        return -EINVAL;
    struct vm_area *curr = prev->vm_next;
    while (curr)
    {
        if (curr->vm_end < p1)
        {
            ;
        }
        else if (p2 < curr->vm_start)
        {
            break;
        }
        else if (p1 <= curr->vm_start && curr->vm_end <= p2)
        {
            debug_printk("[OS] [protect] case3\n");
            // change the pages permissions
            for (unsigned long page_addr = curr->vm_start; page_addr < curr->vm_end; page_addr += FOURKB)
                change_permissions_page(current, page_addr, prot);
            // change the block
            curr->access_flags = prot;
            // merge back or front if possible
            char mergedFront = merge_front(prev, curr);
            if (mergedFront)
            {
                merge_front(prev, prev->vm_next);
                curr = prev;
            }
            else
            {
                merge_front(curr, curr->vm_next);
            }
        }
        else if (p1 <= curr->vm_start && p2 < curr->vm_end)
        {
            if (curr->access_flags != prot)
            {
                debug_printk("[OS] [protect] case4\n");
                for (unsigned long page_addr = curr->vm_start; page_addr < p2; page_addr += FOURKB)
                    change_permissions_page(current, page_addr, prot);
                // split block at front
                struct vm_area *new_vma = make_new_vma(curr->vm_start, p2, curr, prot);
                prev->vm_next = new_vma;
                curr->vm_start = p2;
                // merge front if possible
                merge_front(prev, new_vma);
            }
        }
        else if (curr->vm_start < p1 && curr->vm_end <= p2)
        {
            if (curr->access_flags != prot)
            {
                debug_printk("[OS] [protect] case5\n");
                for (unsigned long page_addr = p1; page_addr < curr->vm_end; page_addr += FOURKB)
                    change_permissions_page(current, page_addr, prot);
                // split block at back
                struct vm_area *new_vma = make_new_vma(p1, curr->vm_end, curr->vm_next, prot);
                curr->vm_next = new_vma;
                curr->vm_end = p1;
                // merge back if possible
                merge_front(new_vma, new_vma->vm_next);
                curr = new_vma;
            }
        }
        else if (curr->vm_start < p1 && p2 < curr->vm_end)
        {
            if (curr->access_flags != prot)
            {
                debug_printk("[OS] [protect] case6\n");
                for (unsigned long page_addr = p1; page_addr < p2; page_addr += FOURKB)
                    change_permissions_page(current, page_addr, prot);
                struct vm_area *new_vma = make_new_vma(p1, p2, NULL, prot);
                struct vm_area *second_curr = make_new_vma(p2, curr->vm_end, curr->vm_next, curr->access_flags);
                new_vma->vm_next = second_curr;
                curr->vm_end = p1;
                curr->vm_next = new_vma;
                curr = new_vma;
                // no merging possible
            }
        }
        // move forward
        prev = curr;
        curr = curr->vm_next;
    }
    return 0;
}

unsigned long join_vm_area(struct vm_area *curr, int length, int prot)
{
    unsigned long mmap_start = curr->vm_end;
    unsigned long mmap_end = mmap_start + length;
    char merged = 0;

    // check if curr can merge
    if (curr->access_flags == prot)
    {
        debug_printk("[OS] Merging front\n");
        curr->vm_end = mmap_end;
        merged = 1;
    }
    // check if next can merge
    char nextCanMerge = (curr->vm_next != NULL) && (curr->vm_next->vm_start == mmap_end);
    if (nextCanMerge && (curr->vm_next->access_flags == prot))
    {
        if (merged == 1)
        {
            debug_printk("[OS] Merging back, to front; deleting node\n");
            curr->vm_end = curr->vm_next->vm_end;
            // remove the next node
            struct vm_area *nextToNext = curr->vm_next->vm_next;
            remove_vma(curr->vm_next);
            curr->vm_next = nextToNext;
            merged = 2;
        }
        else
        {
            debug_printk("[OS] Merging back only\n");
            // extend curr->vm_next to include mmap_start
            curr->vm_next->vm_start = mmap_start;
            merged = 1;
        }
    }
    if (merged == 0)
    {
        debug_printk("[OS] Creating new node of type 0\n");
        struct vm_area *new_vma = make_new_vma(mmap_start, mmap_end, curr->vm_next, prot);
        curr->vm_next = new_vma;
    }
    return mmap_start;
}

/**
 ** mmap system call implementation.
 */
long vm_area_map(struct exec_context *current, u64 addr, int length, int prot, int flags)
{
    addr = page_aligned_start_addr(addr);
    length = page_aligned_end_addr(length);
    if ((prot != PROT_READ) && (prot != (PROT_READ | PROT_WRITE)))
        return -EINVAL;
    if ((flags != MAP_FIXED) && (flags != 0))
        return -EINVAL;

    // MMAP_AREA_START to MMAP_AREA_END
    struct vm_area *vma_head = current->vm_area;
    if (vma_head == NULL)
    {
        debug_printk("[OS] Adding head at %x\n", MMAP_AREA_START);
        // create a dummy head node
        vma_head = make_new_vma(MMAP_AREA_START, MMAP_AREA_START + FOURKB, NULL, 0x0);
        current->vm_area = vma_head;
    }

    struct vm_area *prev = NULL;
    struct vm_area *curr = vma_head;
    if (addr == 0)
    {
        if (flags == MAP_FIXED)
            return -EINVAL;
        while (curr)
        {
            // find the smallest address capable of delivering
            if (MMAP_AREA_END - curr->vm_end >= length)
            {
                if (curr->vm_next)
                {
                    // there is a next block
                    // check if compatible
                    if (curr->vm_next->vm_start - curr->vm_end >= length)
                    {
                        return join_vm_area(curr, length, prot);
                    }
                    else
                    {
                        ; // continue to next iteration
                    }
                }
                else
                {
                    // no other node after curr
                    return join_vm_area(curr, length, prot);
                }
            }
            // move forward
            curr = curr->vm_next;
        }
    }
    else
    {
        if (addr + length > MMAP_AREA_END)
            return -EINVAL;
        debug_printk("[OS] non-null addr called\n");
        while (curr)
        {
            if (curr->vm_end <= addr)
            {
                if (curr->vm_next)
                {
                    if (curr->vm_next->vm_start >= addr + length)
                    {
                        if (curr->vm_end == addr)
                        {
                            // front can merge, along with back considered
                            return join_vm_area(curr, length, prot);
                        }
                        else if (curr->vm_next->vm_start == addr + length)
                        {
                            // end can merge
                            if (curr->vm_next->access_flags == prot)
                            {
                                debug_printk("[OS] Merging back only\n");
                                // merge
                                curr->vm_next->vm_start = addr;
                                return addr;
                            }
                            else
                            {
                                debug_printk("[OS] Creating new node of type 1\n");
                                // can't merge, new block
                                struct vm_area *new_vma = make_new_vma(addr, addr + length, curr->vm_next, prot);
                                curr->vm_next = new_vma;
                                return addr;
                            }
                        }
                        else
                        {
                            debug_printk("[OS] Creating new node type 2\n");
                            // just create a new block
                            struct vm_area *new_vma = make_new_vma(addr, addr + length, curr->vm_next, prot);
                            curr->vm_next = new_vma;
                            return addr;
                        }
                    }
                    else
                    {
                        ; // continue to next iteration
                    }
                }
                else
                {
                    // no other node after curr
                    debug_printk("[OS] Creating new node type 3\n");
                    struct vm_area *new_vma = make_new_vma(addr, addr + length, curr->vm_next, prot);
                    curr->vm_next = new_vma;
                    return addr;
                }
            }
            else
            {
                // addr is now past aka MAP_FIXED not applicable
                if (flags == MAP_FIXED)
                    return -EINVAL;
                else
                    return vm_area_map(current, 0, length, prot, 0);
            }
            // move forward
            curr = curr->vm_next;
        }
    }
}

/**
 * Unallocates physical memory,
 * doesn't unallocate interim page tables even if all of their entries have present 0
 */
void unallocate_page(struct exec_context *current, u64 addr)
{
    addr = page_aligned_start_addr(addr);
    u64 pgd_idx = PGD_IDX(addr);
    u64 pud_idx = PUD_IDX(addr);
    u64 pmd_idx = PMD_IDX(addr);
    u64 pte_idx = PTE_IDX(addr);
    char present = 1;
    u64 *pgd = osmap(current->pgd);
    present = present && (pgd[pgd_idx] & 0x1); // pud present
    if (!present)
        return;
    u64 *pud = osmap(pgd[pgd_idx] >> 12);
    present = present && (pud[pud_idx] & 0x1); // pmd present
    if (!present)
        return;
    u64 *pmd = osmap(pud[pud_idx] >> 12);
    present = present && (pmd[pmd_idx] & 0x1); // pte present
    if (!present)
        return;
    u64 *pte = osmap(pmd[pmd_idx] >> 12);      // page itself
    present = present && (pte[pte_idx] & 0x1); // page present
    if (!present)
        return;
    u64 pfn = pte[pte_idx] >> 12;
    if (get_pfn_refcount(pfn) <= 1)
        os_pfn_free(USER_REG, pfn);
    pte[pte_idx] &= ~(0b1);
    invlpg(addr);
}

/**
 ** munmap system call implemenations
 */
long vm_area_unmap(struct exec_context *current, u64 addr, int length)
{
    /**
     *       p1       p2       p3       p4       p5
     * --|--------|--------|--------|--------|--------|-
     *          |                     |
     *         addr              addr + length
     * such a call would free p1, p2, p3, p4
     */
    u64 p1 = page_aligned_start_addr(addr);
    u64 p2 = page_aligned_end_addr(addr + length);
    struct vm_area *prev = current->vm_area;
    if (prev == NULL)
        return 0; // no mmap called
    struct vm_area *curr = prev->vm_next;
    while (curr)
    {
        if (curr->vm_end < p1)
        {
            ; // do nothing
        }
        else if (p2 < curr->vm_start)
        {
            break; // past the unmap range
        }
        else if (p1 <= curr->vm_start && curr->vm_end <= p2)
        {
            // delete this node
            // unallocate physical memory allocation
            for (unsigned long page_addr = curr->vm_start; page_addr < curr->vm_end; page_addr += FOURKB)
                unallocate_page(current, page_addr);
            // now change virtual address allocation
            struct vm_area *next = curr->vm_next;
            remove_vma(curr);
            prev->vm_next = next;
        }
        else if (p1 <= curr->vm_start && p2 < curr->vm_end)
        {
            // shrink from front
            // unallocate physical memory allocation
            for (unsigned long page_addr = curr->vm_start; page_addr < p2; page_addr += FOURKB)
                unallocate_page(current, page_addr);
            // now change virtual address allocation
            curr->vm_start = p2;
        }
        else if (curr->vm_start < p1 && curr->vm_end <= p2)
        {
            // shrink from back
            // unallocate physical memory allocation
            for (unsigned long page_addr = p1; page_addr < curr->vm_end; page_addr += FOURKB)
                unallocate_page(current, page_addr);
            // now change virtual address allocation
            curr->vm_end = p1;
        }
        else if (curr->vm_start < p1 && p2 < curr->vm_end)
        {
            // unallocate physical memory allocation
            for (unsigned long page_addr = p1; page_addr < p2; page_addr += FOURKB)
                unallocate_page(current, page_addr);
            // break in two
            struct vm_area *new_vma = make_new_vma(p2, curr->vm_end, curr->vm_next, curr->access_flags);
            curr->vm_end = p1;
            curr->vm_next = new_vma;
        }
        // move forward
        prev = curr;
        curr = curr->vm_next;
    }
    return 0;
}

/**
 * Increases the reference count of the actual final page frame
 * @param current current context
 * @param addr virtual address
 * @param prot protection flag, either PROT_READ or PROT_READ | PROT_WRITE
 */
int allocate_page(struct exec_context *current, u64 addr, int prot)
{
    u64 pgd_idx = PGD_IDX(addr);
    u64 pud_idx = PUD_IDX(addr);
    u64 pmd_idx = PMD_IDX(addr);
    u64 pte_idx = PTE_IDX(addr);
    // debug_printk("[OS] [Fault] %x %x %x %x\n", pgd_idx, pud_idx, pmd_idx, pte_idx);
    //? assuming read access is always allowed, either user or os
    //* 0b1001 means user accessible present page
    char write_access = (prot & PROT_WRITE) << 2;
    u64 pte_access = write_access | 0b10001;
    char present = 1;

    u64 *pgd = osmap(current->pgd);
    present = present && (pgd[pgd_idx] & 0x1);
    if (!present)
    {
        // pgd level doesn't have the next entry
        u64 new_pfn = os_pfn_alloc(OS_PT_REG);
        if (new_pfn < 0)
            return -ENOMEM;
        pgd[pgd_idx] = (new_pfn << 12) | pte_access;
        present = 0;
    }
    else
    {
        // this is to take into account the access at each hierarchy
        pgd[pgd_idx] |= write_access;
    }
    u64 *pud = osmap(pgd[pgd_idx] >> 12);
    present = present && (pud[pud_idx] & 0x1);
    if (!present)
    {
        // pud level doesn't have the next entry
        u64 new_pfn = os_pfn_alloc(OS_PT_REG);
        if (new_pfn < 0)
            return -ENOMEM;
        pud[pud_idx] = (new_pfn << 12) | pte_access;
        present = 0;
    }
    else
    {
        pud[pud_idx] |= write_access;
    }
    u64 *pmd = osmap(pud[pud_idx] >> 12);
    present = present && (pmd[pmd_idx] & 0x1);
    if (!present)
    {
        // pmd level doesn't have the next entry
        u64 new_pfn = os_pfn_alloc(OS_PT_REG);
        if (new_pfn < 0)
            return -ENOMEM;
        pmd[pmd_idx] = (new_pfn << 12) | pte_access;
        present = 0;
    }
    else
    {
        pmd[pmd_idx] |= write_access;
    }
    u64 *pte = osmap(pmd[pmd_idx] >> 12);
    present = present && (pte[pte_idx] & 0x1);
    if (!present)
    {
        // pte level doesn't have the next entry
        u64 pte_pfn = os_pfn_alloc(USER_REG);
        if (pte_pfn < 0)
            return -ENOMEM;
        pte[pte_idx] = (pte_pfn << 12) | pte_access;
    }
    else
    {
        pte[pte_idx] |= write_access;
    }
    return 0;
}

/**
 * Function will invoked whenever there is page fault for an address in the vm area region
 * created using mmap
 */
long vm_area_pagefault(struct exec_context *current, u64 addr, int error_code)
{
    addr = page_aligned_start_addr(addr); // this page is wanted
    error_code = error_code & 0b111;
    debug_printk("[OS] [Fault] error_code: %x, addr: %x\n", error_code, addr);
    struct vm_area *curr = current->vm_area;
    if (curr == NULL)
        return -EINVAL;   // mmap never called
    curr = curr->vm_next; // start from actual vm_areas, after dummy
    while (curr)
    {
        if (curr->vm_start <= addr && addr < curr->vm_end)
            break;
        // move forward
        curr = curr->vm_next;
    }
    if (curr == NULL)
    {
        debug_printk("[OS] [Fault] Case 2\n");
        return -EINVAL; // case 2
    }
    if (error_code == 0x7)
    {
        // case 3
        debug_printk("[OS] [Fault] Case 3 access: %x\n", curr->access_flags);
        if ((curr->access_flags & PROT_WRITE) != PROT_WRITE)
            return -EINVAL;
        debug_printk("[OS] [Fault] Case 3 CoW\n");
        return handle_cow_fault(current, addr, curr->access_flags);
    }
    if (curr->access_flags == PROT_READ)
    {
        debug_printk("[OS] [Fault] Case 1 Write possible %x\n", error_code);
        if ((error_code & 0b10) == 0b10)
        {
            debug_printk("[OS] [Fault] Case 1 Write in Read Only\n");
            return -EINVAL; // write access on read only page
        }
    }
    // case 1
    debug_printk("[OS] [Fault] Case 1\n");
    allocate_page(current, addr, curr->access_flags);
    debug_printk("[OS] [Fault] Handled\n");
    return 1;
}

/**
 * Creates a new translation of a page corresponding to parent's page table entry,
 * stops doing so if parent's page entry has present bit 0 at any level.
 * Turns off write bit in every level of parent
 * then copies access rights/bits as is from the parent
 *
 * @param child current context
 * @param parent parent context
 * @param addr virtual address
 */
int cfork_copy_page(struct exec_context *child, struct exec_context *parent, u64 addr)
{
    u64 pgd_idx = PGD_IDX(addr);
    u64 pud_idx = PUD_IDX(addr);
    u64 pmd_idx = PMD_IDX(addr);
    u64 pte_idx = PTE_IDX(addr);
    u64 pte_access = 0b10001;
    char present = 1;

    u64 *pgd = osmap(parent->pgd);
    if (!(pgd[pgd_idx] & 0x1))
        return 0;
    pgd[pgd_idx] = pgd[pgd_idx] & (~(0b1000));   // turn off write bit in parent
    pte_access = pgd[pgd_idx] & ((1 << 12) - 1); // copy access bits from parent
    u64 *cpgd = osmap(child->pgd);
    present = present && (cpgd[pgd_idx] & 0x1);
    if (!present)
    {
        // pgd level doesn't have the next entry
        u64 new_pfn = os_pfn_alloc(OS_PT_REG);
        if (new_pfn < 0)
            return -ENOMEM;
        cpgd[pgd_idx] = (new_pfn << 12) | pte_access;
        present = 0;
    }
    else
    {
        cpgd[pgd_idx] |= pte_access;
    }

    u64 *pud = osmap(pgd[pgd_idx] >> 12);
    if (!(pud[pud_idx] & 0x1))
        return 0;
    pud[pud_idx] = pud[pud_idx] & ~(0b1000);
    pte_access = pud[pud_idx] & ((1 << 12) - 1);
    u64 *cpud = osmap(cpgd[pgd_idx] >> 12);
    present = present && (cpud[pud_idx] & 0x1);
    if (!present)
    {
        // pud level doesn't have the next entry
        u64 new_pfn = os_pfn_alloc(OS_PT_REG);
        if (new_pfn < 0)
            return -ENOMEM;
        cpud[pud_idx] = (new_pfn << 12) | pte_access;
        present = 0;
    }
    else
    {
        cpud[pud_idx] |= pte_access;
    }

    u64 *pmd = osmap(pud[pud_idx] >> 12);
    if (!(pmd[pmd_idx] & 0x1))
        return 0;
    pmd[pmd_idx] = pmd[pmd_idx] & ~(0b1000);
    pte_access = pmd[pmd_idx] & ((1 << 12) - 1);
    u64 *cpmd = osmap(cpud[pud_idx] >> 12);
    present = present && (cpmd[pmd_idx] & 0x1);
    if (!present)
    {
        // pmd level doesn't have the next entry
        u64 new_pfn = os_pfn_alloc(OS_PT_REG);
        if (new_pfn < 0)
            return -ENOMEM;
        cpmd[pmd_idx] = (new_pfn << 12) | pte_access;
        present = 0;
    }
    else
    {
        cpmd[pmd_idx] |= pte_access;
    }

    u64 *pte = osmap(pmd[pmd_idx] >> 12);
    if (!(pte[pte_idx] & 0x1))
        return 0;
    pte[pte_idx] = pte[pte_idx] & ~(0b1000);
    pte_access = pte[pte_idx] & ((1 << 12) - 1);
    u64 *cpte = osmap(cpmd[pmd_idx] >> 12);
    u64 pfn = (pte[pte_idx] >> 12);
    cpte[pte_idx] = (pfn << 12) | pte_access;
    get_pfn(pfn);
    invlpg(pfn);
    return 0;
}

/**
 ** cfork system call implementations
 * The parent returns the pid of child process. The return path of
 * the child process is handled separately through the calls at the
 * end of this function (e.g., setup_child_context etc.)
 */
long do_cfork()
{
    u32 pid;
    struct exec_context *new_ctx = get_new_ctx();
    struct exec_context *ctx = get_current_ctx();
    /**
     *! Do not modify above lines
     */
    /*---------------- Your code [start]---------------*/

    pid = new_ctx->pid;
    debug_printk("[OS] [cfork] new pid: %x\n", pid);
    memcpy(new_ctx, ctx, sizeof(struct exec_context));
    new_ctx->pid = pid;
    new_ctx->ppid = ctx->pid;
    // copy vma linked list
    struct vm_area *curr = ctx->vm_area;
    struct vm_area *new_curr = new_ctx->vm_area;
    if (curr)
    {
        new_ctx->vm_area = make_new_vma(curr->vm_start, curr->vm_end, NULL, curr->access_flags);
        new_curr = new_ctx->vm_area;
        while (curr->vm_next)
        {
            // move forward
            curr = curr->vm_next;
            new_curr->vm_next = make_new_vma(curr->vm_start, curr->vm_end, NULL, curr->access_flags);
            new_curr = new_curr->vm_next;
        }
    }
    /*
    // copy files
    for (int i = 0; i < MAX_OPEN_FILES; i++)
    {
        new_ctx->files[i] = os_alloc(sizeof(struct file));
        memcpy(new_ctx->files[i], ctx->files[i], sizeof(struct file));
    }
    */

    // create PGD
    u64 new_pfn = os_pfn_alloc(OS_PT_REG);
    new_ctx->pgd = new_pfn;
    debug_printk("[OS] [cfork] Allocated pgd %x\n", new_pfn);

    u64 start, end;
    start = page_aligned_start_addr(new_ctx->mms[MM_SEG_CODE].start);
    end = page_aligned_end_addr(new_ctx->mms[MM_SEG_CODE].next_free);
    debug_printk("[OS] [cfork] CODE %x to %x\n", start, end);
    for (u64 page = start; page < end; page += FOURKB)
    {
        char c = cfork_copy_page(new_ctx, ctx, page);
        if (c < 0)
        {
            os_free(new_ctx, sizeof(struct exec_context));
            return -EINVAL;
        }
    }
    start = page_aligned_start_addr(new_ctx->mms[MM_SEG_RODATA].start);
    end = page_aligned_end_addr(new_ctx->mms[MM_SEG_RODATA].next_free);
    debug_printk("[OS] [cfork] RODATA %x to %x\n", start, end);
    for (u64 page = start; page < end; page += FOURKB)
    {
        char c = cfork_copy_page(new_ctx, ctx, page);
        if (c < 0)
        {
            os_free(new_ctx, sizeof(struct exec_context));
            return -EINVAL;
        }
    }
    start = page_aligned_start_addr(new_ctx->mms[MM_SEG_DATA].start);
    end = page_aligned_end_addr(new_ctx->mms[MM_SEG_DATA].next_free);
    debug_printk("[OS] [cfork] DATA %x to %x\n", start, end);
    for (u64 page = start; page < end; page += FOURKB)
    {
        char c = cfork_copy_page(new_ctx, ctx, page);
        if (c < 0)
        {
            os_free(new_ctx, sizeof(struct exec_context));
            return -EINVAL;
        }
    }
    start = page_aligned_start_addr(new_ctx->mms[MM_SEG_STACK].start);
    end = page_aligned_end_addr(new_ctx->mms[MM_SEG_STACK].end);
    debug_printk("[OS] [cfork] STACK %x to %x\n", start, end);
    for (u64 page = start; page < end; page += FOURKB)
    {
        char c = cfork_copy_page(new_ctx, ctx, page);
        if (c < 0)
        {
            os_free(new_ctx, sizeof(struct exec_context));
            return -EINVAL;
        }
    }
    new_curr = new_ctx->vm_area;
    while (new_curr)
    {
        debug_printk("[OS] [cfork] VMA %x to %x\n", start, end);
        for (u64 page = new_curr->vm_start; page < new_curr->vm_end; page += FOURKB)
        {
            char c = cfork_copy_page(new_ctx, ctx, page);
            if (c < 0)
            {
                os_free(new_ctx, sizeof(struct exec_context));
                return -EINVAL;
            }
        }
        // move forward
        new_curr = new_curr->vm_next;
    }

    /*---------------- Your code [end] ----------------*/

    /**
     *! The remaining part must not be changed
     */
    copy_os_pts(ctx->pgd, new_ctx->pgd);
    do_file_fork(new_ctx);
    setup_child_context(new_ctx);
    return pid;
}

/**
 * Cow fault handling, for the entire user address space
 * For address belonging to memory segments (i.e., stack, data)
 * it is called when there is a CoW violation in these areas.
 *
 * For vm areas, your fault handler 'vm_area_pagefault'
 * should invoke this function
 * */
long handle_cow_fault(struct exec_context *current, u64 vaddr, int access_flags)
{
    debug_printk("[OS] [Fault] [CoW] pid: %x, access_flags: %x, addr: %x\n", current->pid, access_flags, vaddr);
    u64 pgd_idx = PGD_IDX(vaddr);
    u64 pud_idx = PUD_IDX(vaddr);
    u64 pmd_idx = PMD_IDX(vaddr);
    u64 pte_idx = PTE_IDX(vaddr);
    char write_access = 0b1000;

    u64 *pgd = osmap(current->pgd);
    pgd[pgd_idx] |= write_access;
    u64 *pud = osmap(pgd[pgd_idx] >> 12);
    pud[pud_idx] |= write_access;
    u64 *pmd = osmap(pud[pud_idx] >> 12);
    pmd[pmd_idx] |= write_access;
    u64 *pte = osmap(pmd[pmd_idx] >> 12);

    u64 pte_pfn = os_pfn_alloc(USER_REG);
    if (pte_pfn < 0)
        return -EINVAL;
    put_pfn(pte[pte_idx] >> 12);
    memcpy(osmap(pte_pfn), osmap(pte[pte_idx] >> 12), FOURKB);
    pte[pte_idx] = (pte_pfn << 12) | (pte[pte_idx] & ((1 << 12) - 1)) | write_access;
    return 1;
}
