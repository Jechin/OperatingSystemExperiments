#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

#define default_mem_size 1024
#define first_fit_algorithm 1
#define best_fit_algorithm 2
#define worst_fit_algorithm 3

typedef struct free_block
{
    int size;
    int start_address;
    free_block *next;
} free_block;

typedef struct allocated_block
{
    int pid;
    int size;
    int start_address;
    string process_name;
    allocated_block *next;
} allocated_block;

/**
 * @brief 空闲块链表
 */
free_block *free_block_list;
/**
 * @brief 已分配的块链表
 */
allocated_block *allocated_block_list;
/**
 * @brief 当前创建进程的pid
 * 为防止重复pid
 * pid_number会在创建一个进程后自加一
 */
int pid_number;
/**
 * @brief 是否可以设置存储大小
 * 0-不可以
 * 1-可以
 */
int setmem_flag;
/**
 * @brief 当前的分配算法
 */
int algorithm;

void Initial();
void menu();
void set_mem_size();
void set_algorithm();
int arrange(int al);
void swap_free(free_block *a, free_block *b);
void new_process();
void display_mem();
void check_algorithm();
int allocate_block(int size);
void clean_zero();
void kill_process();
void merge_block();

int main()
{
    Initial();
    int choice;

    while (true)
    {
        menu();
        cin >> choice;
        switch (choice)
        {
        case 1:
            set_mem_size();
            break;
        case 2:
            set_algorithm();
            break;
        case 3:
            new_process();
            break;
        case 4:
            kill_process();
            break;
        case 5:
            display_mem();
            break;
        case 0:
            exit(0);
        }
    }
    return 0;
}

/**
 * @brief 初始化函数
 * 以默认大小设置存储器
 * 默认存储器分配算法为最先适应算法
 * @param {*}
 * @return {*}
 */
void Initial()
{
    free_block_list = (free_block *)malloc(sizeof(free_block));
    free_block_list->size = default_mem_size;
    free_block_list->next = nullptr;
    free_block *ftemp = (free_block *)malloc(sizeof(free_block));
    ftemp->size = default_mem_size;
    ftemp->next = free_block_list->next;
    free_block_list->next = ftemp;

    allocated_block_list = (allocated_block *)malloc(sizeof(allocated_block));
    allocated_block_list->next = nullptr;

    pid_number = 1;
    setmem_flag = 1;
    algorithm = first_fit_algorithm;
    cout << "-------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief 输出菜单相关内容
 * @param {*}
 * @return {*}
 */
void menu()
{
    cout << "1. Set mem size" << endl;
    cout << "2. Set algorithm" << endl;
    cout << "3. New process" << endl;
    cout << "4. Kill a process" << endl;
    cout << "5. Display mem" << endl;
    cout << "0. Exit" << endl;
    cout << "Input a number: ";
    return;
}

/**
 * @brief 设置存储器大小
 * 存储器大小只能设置一次
 * 且有进程分配空闲块后不可更改存储器大小
 * @param {*}
 * @return {*}
 */
void set_mem_size()
{
    cout << "-------------------------------------------------------------------------" << endl;
    if (setmem_flag)
    {
        int size = 1024;
        cout << "Input memory size(default 1024): ";
        cin >> size;
        free_block_list->size = size;
        free_block_list->next->size = size;
        setmem_flag = 0;
    }
    else
    {
        cout << "Cannot set mem size repeatedly." << endl;
    }
    cout << "-------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief 设置存储器的分配算法
 * @param {*}
 * @return {*}
 */
void set_algorithm()
{
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "Set algorithm (default first fit algorithm):" << endl;
    cout << "1. First fit algorithm" << endl;
    cout << "2. Best fit algorithm" << endl;
    cout << "3. Worst fit algorithm" << endl;
    int res;
    cin >> res;
    if (res > 0 && res < 4)
        algorithm = res;

    res = arrange(algorithm);
    if (res == algorithm)
        cout << "Set algorithm successfully." << endl;
    cout << "-------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief 根据分配算法重新排列
 * 最先适应算法：根据空闲块起始地址升序排列
 * 最佳适应算法：根据空闲块大小生序排列
 * 最坏适应算法：根据空闲块大小降序排列
 * @param {int} al 分配算法
 * @return {*}
 */
int arrange(int al)
{
    free_block *temp = free_block_list;
    free_block *ftemp = temp->next;
    if (ftemp == nullptr || ftemp->next == nullptr)
        return al;
    temp = ftemp;
    ftemp = ftemp->next;
    while (temp != nullptr)
    {
        ftemp = temp->next;
        while (ftemp != nullptr)
        {

            if (al == 1)
            {
                if (temp->start_address > ftemp->start_address)
                    swap_free(temp, ftemp);
                ftemp = ftemp->next;
            }
            if (al == 2)
            {
                if (temp->size > ftemp->size)
                    swap_free(temp, ftemp);
                if (temp->size == ftemp->size)
                {
                    if (temp->start_address > ftemp->start_address)
                        swap_free(temp, ftemp);
                }
                ftemp = ftemp->next;
            }
            if (al == 3)
            {
                if (temp->size < ftemp->size)
                    swap_free(temp, ftemp);
                if (temp->size == ftemp->size)
                {
                    if (temp->start_address > ftemp->start_address)
                        swap_free(temp, ftemp);
                }
                ftemp = ftemp->next;
            }
        }
        temp = temp->next;
    }
    return al;
}

/**
 * @brief: 交换两个空闲块在链表中位置
 * 实现过程并未改动指针
 * 知识替换两个节点中的信息（大小、起始地址）
 * @param {free_block} *a
 * @param {free_block} *b
 * @return {*}
 */
void swap_free(free_block *a, free_block *b)
{
    int temp;
    temp = a->size;
    a->size = b->size;
    b->size = temp;

    temp = a->start_address;
    a->start_address = b->start_address;
    b->start_address = temp;

    return;
}

/**
 * @brief 输出所有存储块的情况
 * @param {*}
 * @return {*}
 */
void display_mem()
{
    cout << "-------------------------------------------------------------------------" << endl;
    cout << "free block:" << endl;
    cout << setw(20) << "start_addr" << setw(15) << "size" << endl;

    free_block *ftemp = free_block_list->next;
    if (ftemp == nullptr)
        cout << setw(20) << "null" << setw(15) << "null" << endl;
    else
    {
        while (ftemp != nullptr)
        {
            cout << setw(20) << ftemp->start_address;
            cout << setw(15) << ftemp->size;
            cout << endl;

            ftemp = ftemp->next;
        }
    }
    cout << endl;
    cout << "allocated block:" << endl;
    cout << setw(10) << "pid" << setw(20) << "proc_name" << setw(20) << "start_addr" << setw(15) << "size" << endl;
    allocated_block *atemp = allocated_block_list->next;
    if (atemp == nullptr)
        cout << setw(10) << "null" << setw(20) << "null" << setw(20) << "null" << setw(15) << "null" << endl;
    else
    {
        while (atemp != nullptr)
        {
            cout << setw(10) << atemp->pid;
            cout << setw(20) << atemp->process_name;
            cout << setw(20) << atemp->start_address;
            cout << setw(15) << atemp->size;
            cout << endl;
            atemp = atemp->next;
        }
    }
    cout << "-------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief 创建新的进程并分配存储块
 * @param {*}
 * @return {*}
 */
void new_process()
{
    cout << "-------------------------------------------------------------------------" << endl;
    allocated_block *atemp = (allocated_block *)malloc(sizeof(allocated_block));
    atemp->pid = pid_number;
    cout << "Input process name: ";
    cin >> atemp->process_name;
    cout << "Input mem size needed: ";
    cin >> atemp->size;

    check_algorithm();
    int addr = allocate_block(atemp->size);
    if (addr == -1)
    {
        cout << "Allocate memmory failed!" << endl;
        return;
    }
    setmem_flag = 0;
    pid_number++;
    atemp->start_address = addr;
    atemp->next = allocated_block_list->next;
    allocated_block_list->next = atemp;
    cout << "-------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief 输出当前的分配算法
 * @param {*}
 * @return {*}
 */
void check_algorithm()
{
    switch (algorithm)
    {
    case 1:
        cout << "Now using first fit algorithm." << endl;
        break;
    case 2:
        cout << "Now using best fit algorithm." << endl;
        break;
    case 3:
        cout << "Now using worst fit algorithm." << endl;
        break;
    }
    return;
}

/**
 * @brief 进程分配存储块函数
 * 从空闲块链表头部开始查找直至符合情况
 * 空闲块链表已按照算法进行排列
 * @param {int} size 所需要的小大
 * @return {int} addr 分配的起始地址
 */
int allocate_block(int size)
{
    int addr = -1;
    free_block *ftemp = free_block_list->next;
    while (ftemp != nullptr)
    {
        if (ftemp->size >= size)
        {
            addr = ftemp->start_address;
            ftemp->start_address += size;
            ftemp->size -= size;
            break;
        }
        ftemp = ftemp->next;
    }
    clean_zero();
    arrange(algorithm);
    return addr;
}

/**
 * @brief 删除空闲块链表中大小为0的空闲块
 * @param {*}
 * @return {*}
 */
void clean_zero()
{
    free_block *temp = free_block_list;
    free_block *ftemp = temp->next;
    while (ftemp != nullptr)
    {
        if (ftemp->size == 0)
        {
            temp->next = ftemp->next;
            ftemp = ftemp->next;
        }
        else
        {
            temp = ftemp;
            ftemp = ftemp->next;
        }
    }
    return;
}

/**
 * @brief 杀死一个进程并释放所分配的存储空间
 * 在已分配块链表中删除
 * 新创建一个空闲块插入空闲块链表中
 * @param {*}
 * @return {*}
 */
void kill_process()
{
    cout << "-------------------------------------------------------------------------" << endl;
    int pid, flag = 0;
    cout << "Input pid want to kill: ";
    cin >> pid;

    allocated_block *temp = allocated_block_list;
    allocated_block *atemp = allocated_block_list->next;
    while (atemp != nullptr)
    {
        if (atemp->pid == pid)
        {
            free_block *ftemp = (free_block *)malloc(sizeof(free_block));
            ftemp->start_address = atemp->start_address;
            ftemp->size = atemp->size;

            ftemp->next = free_block_list->next;
            free_block_list->next = ftemp;

            temp->next = atemp->next;
            atemp = atemp->next;

            flag = 1;
            break;
        }
        temp = atemp;
        atemp = atemp->next;
    }

    if (flag == 0)
    {
        cout << "No such process allocated memory." << endl;
        return;
    }

    merge_block();

    cout << "-------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief: 合并空闲块
 * 按起始地址排序
 * 合并空闲块
 * 按算法重新排序
 * @param {*}
 * @return {*}
 */
void merge_block()
{
    arrange(1);
    free_block *temp = free_block_list->next;
    free_block *ftemp = temp->next;
    while (ftemp != nullptr)
    {
        if (ftemp->start_address == temp->start_address + temp->size)
        {
            temp->size += ftemp->size;
            temp->next = ftemp->next;
            ftemp = ftemp->next;
        }
        else
        {
            temp = ftemp;
            ftemp = ftemp->next;
        }
    }
    arrange(algorithm);
    return;
}