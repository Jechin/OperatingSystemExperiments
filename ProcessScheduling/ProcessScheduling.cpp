#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

/**
 * @brief 进程的结构体
 * @param {
 * pid      进程标识
 * resouce  进程所需资源标识
 * pstatus  进程状态（就绪、运行、阻塞）
 * priority 进程优先级
 * step     进程执行剩余所需时间
 * wait     进程等待时间
 * }
 */
typedef struct process
{
    string pid;
    string resouce;
    string pstatus;
    int priority;
    int step;
    int wait;
} process;

/**
 * @brief 进程链表节点
 * @param {proc 进程指针
 * next 下一个进程指针}
 */
typedef struct process_node
{
    process *proc;
    process_node *next;
} process_node;

/**
 * @brief 资源结构体
 * @param {rid      资源标识;
 * rstatus  资源状态（空闲、已分配）;
 * wait     等待该资源的进程序列（链表）}
 */
typedef struct resouce
{
    string rid;
    string rstatus;
    process_node *wait;
} resouce;

/**
 * @brief 资源链表节点
 */
typedef struct resouce_node
{
    resouce *resc;
    resouce_node *next;
} resouce_node;

/**
 * @brief 所有进程组成的链表
 */
process_node *process_list;
/**
 * @brief 就绪状态的进程的链表
 */
process_node *process_ready;
/**
 * @brief 所有资源组成的链表
 */
resouce_node *resouce_list;
/**
 * @brief cpu当前在运行的进程
 */
process *cpu;
/**
 * @brief 进程数
 */
int process_number;
/**
 * @brief 资源数
 */
int resouce_number;

process_node *InitProcess(process_node *list);
resouce_node *InitResouce(resouce_node *list);
void create_process();
int check_pid(string pid);
int check_rid(string rid);
void insert_process_list(process *p);
void insert_process_ready(process *p);
void create_resouce();
void get_process_info(process *p);
void show_process(process_node *pnode);
void show_cpu();
void delete_process();
void delete_resouce();
process *get_highest_process();
resouce_node *get_resouce(string rid);
void readytoblock(string rid);
void blocktoready(string rid);
void waitplusone(process *cpu);
void run_one_step();
void runsteps();

/**
 * @brief 初始化进程队列链表函数
 * @param {process_node} *list
 * @return {*}
 */
process_node *InitProcess(process_node *list)
{
    list = (process_node *)malloc(sizeof(process_node));
    list->proc = nullptr;
    list->next = nullptr;

    return list;
}

/**
 * @brief 初始化资源队列链表函数
 * @param {resouce_node} *list
 * @return {*}
 */
resouce_node *InitResouce(resouce_node *list)
{
    list = (resouce_node *)malloc(sizeof(resouce_node));
    list->resc = nullptr;
    list->next = nullptr;

    return list;
}

/**
 * @brief 创建进程函数
 * @param {*}
 * @return {*}
 */
void create_process()
{
    process *proc_temp = (process *)malloc(sizeof(process));
    cout << "Input pid: ";
    cin >> proc_temp->pid;
    cout << "Input resouce needed (input \'no\' if no resouce needed: )";
    cin >> proc_temp->resouce;
    cout << "Input priority: ";
    cin >> proc_temp->priority;
    cout << "Input step: ";
    cin >> proc_temp->step;
    proc_temp->pstatus = "block";
    proc_temp->wait = 0;

    int check;
    check = check_pid(proc_temp->pid); //检查是否有重复的pid
    if (check == 0)
    {
        cout << "Illegal Pid. Create Process Failed." << endl;
        return;
    }

    check = check_rid(proc_temp->resouce); //检查资源是否存在
    if (check == 0)
    {
        cout << "No resouce " << proc_temp->resouce << ". Create Process Failed." << endl;
        return;
    }
    else if (check == 1) //若资源空闲
    {
        proc_temp->pstatus = "ready";    //则该进程创建后状态位就绪
        insert_process_ready(proc_temp); //进程插入就绪队列
    }
    insert_process_list(proc_temp);
    cout << "Create Process Successfully!" << endl;

    return;
}

/**
 * @brief 检查是否存在重复pid
 * @param {string} pid
 * @return 
 *      0:存在重复pid，pid不可用
 *      1:不存在重复pid，pid可用
 */
int check_pid(string pid)
{
    process_node *ptemp = process_list->next;
    while (ptemp != nullptr)
    {
        if (ptemp->proc->pid == pid)
            return 0;
        ptemp = ptemp->next;
    }
    return 1;
}

/**
 * @brief 查看资源rid状态
 * @param {string} rid
 * @return 
 *      0:资源不存在
 *      1:资源空闲
 *      2:资源已被分配
 */
int check_rid(string rid)
{
    resouce_node *rtemp = resouce_list->next;
    while (rtemp != nullptr)
    {
        if (rtemp->resc->rid == rid)
        {
            if (rtemp->resc->rstatus == "free")
                return 1;
            else
                return 2;
        }
        rtemp = rtemp->next;
    }
    return 0;
}

/**
 * @brief 将进程p插入到全局进程队列process_list中
 * @param {process} *p
 * @return {*}
 */
void insert_process_list(process *p)
{
    process_node *pnode = (process_node *)malloc(sizeof(process_node));
    pnode->next = nullptr;
    pnode->proc = p;

    pnode->next = process_list->next;
    process_list->next = pnode;
    return;
}

/**
 * @brief: 将进程p插入到就绪进程队列process_ready中
 * @param {process} *p
 * @return {*}
 */
void insert_process_ready(process *p)
{
    process_node *pnode = (process_node *)malloc(sizeof(process_node));
    pnode->next = nullptr;
    pnode->proc = p;

    pnode->next = process_ready->next;
    process_ready->next = pnode;
    return;
}

/**
 * @brief 创建资源函数
 * @param {*}
 * @return {*}
 */
void create_resouce()
{
    resouce *resc = (resouce *)malloc(sizeof(resouce));
    cout << "Input Rid: ";
    cin >> resc->rid;
    resc->rstatus = "free";

    if (check_rid(resc->rid) != 0) //检查是否有重复rid
    {
        cout << "Input Illegal Rid. Create Resouce Failed." << endl;
        return;
    }

    resouce_node *rnode = (resouce_node *)malloc(sizeof(resouce_node));
    rnode->next = nullptr;
    rnode->resc = resc;

    rnode->next = resouce_list->next;
    resouce_list->next = rnode;
    return;
}

/**
 * @brief 输出进程p的相关信息
 * @param {process} *p
 * @return {*}
 */
void get_process_info(process *p)
{
    if (p == nullptr)
        cout << "show_proc_info error." << endl;
    else
    {
        // printf("%20s %20s %10d %20s %10d\n", p->pid, p->pstatus, p->pstatus, p->resouce, p->step);
        // cout << "pid: " << p->pid << ";  ";
        // cout << "status: " << p->pstatus << ";  ";
        // cout << "priority: " << p->priority << ";  ";
        // cout << "resouce: " << p->resouce << ";  ";
        // cout << "step: " << p->step << endl;

        cout << setw(10) << p->pid;
        cout << setw(10) << p->pstatus;
        cout << setw(10) << p->priority;
        cout << setw(10) << p->resouce;
        cout << setw(10) << p->step;
        cout << setw(10) << p->wait;
        cout << endl;
    }
    return;
}

/**
 * @brief 输出pnode进程队列中所有的进程信息
 * @param {process_node} *pnode
 * @return {*}
 */
void show_process(process_node *pnode)
{
    cout << "-------------------------------------------------------------------------------" << endl;
    process_node *ptemp = pnode->next;
    if (ptemp == nullptr)
    {
        cout << "No process!" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        return;
    }
    //printf("%20s %20s %10s %20s %10s\n", "pid", "status", "priority", "resouce", "step");
    cout << setw(10) << "pid";
    cout << setw(10) << "pstatus";
    cout << setw(10) << "priority";
    cout << setw(10) << "resouce";
    cout << setw(10) << "step";
    cout << setw(10) << "wait" << endl;
    while (ptemp != nullptr)
    {
        get_process_info(ptemp->proc);
        ptemp = ptemp->next;
    }
    cout << "-------------------------------------------------------------------------------" << endl;
    return;
}

/**
 * @brief 输出cpu当前运行的进程信息
 * @param {*}
 * @return {*}
 */
void show_cpu()
{
    if (cpu == nullptr)
        cout << "No Process Running in CPU." << endl;
    else
    {
        //printf("%20s %20s %10s %20s %10s\n", "pid", "status", "priority", "resouce", "step");
        cout << setw(10) << "pid";
        cout << setw(10) << "pstatus";
        cout << setw(10) << "priority";
        cout << setw(10) << "resouce";
        cout << setw(10) << "step";
        cout << setw(10) << "wait" << endl;
        get_process_info(cpu);
    }
    return;
}

/**
 * @brief 删除进程函数
 * @param {*}
 * @return {*}
 */
void delete_process()
{
    string pid;
    cout << "Input pid to delete: ";
    cin >> pid;

    process_node *ptemp = process_list->next;
    process_node *temp = process_list;
    int ready = 0;
    while (ptemp != nullptr)
    {
        if (ptemp->proc->pid == pid)
        {
            if (ptemp->proc->pstatus == "running")
            {
                cout << "Process is Runnig. Delete Failed.\n";
                return;
            }
            if (ptemp->proc->pstatus == "ready")
                ready = 1;
            temp->next = ptemp->next;
            break;
        }
        temp = ptemp;
        ptemp = ptemp->next;
    }

    if (ready)
    {
        ptemp = process_ready->next;
        temp = process_ready;
        while (ptemp != nullptr)
        {
            if (ptemp->proc->pid == pid)
            {
                temp->next = ptemp->next;
                break;
            }
            temp = ptemp;
            ptemp = ptemp->next;
        }
    }
    cout << "Delete Process Successfully!" << endl;
    return;
}

/**
 * @brief 删除资源函数
 * @param {*}
 * @return {*}
 */
void delete_resouce()
{
    string rid;
    cout << "Input rid to delete: ";
    cin >> rid;

    int flag = 1;
    process_node *ptemp = process_list->next;
    while (ptemp != nullptr)
    {
        if (ptemp->proc->resouce == rid) //如果有进程如需要该资源则删除失败
        {
            flag = 0;
            cout << "Delete Resouce Failed." << endl;
            break;
        }
        ptemp = ptemp->next;
    }

    if (flag)
    {
        resouce_node *rtemp = resouce_list->next;
        resouce_node *temp = resouce_list;
        while (rtemp != nullptr)
        {
            if (rtemp->resc->rid == rid)
            {
                temp->next = rtemp->next;
                cout << "Delete Resouce Successfully!" << endl;
                break;
            }
            temp = rtemp;
            rtemp = rtemp->next;
        }
    }
    return;
}

/**
 * @brief 在就绪队列中获取优先级最高的进程
 * 在优先级相同的情况下选择等待时间更久的进程
 * @param {*}
 * @return {*}
 * 返回优秀级最高的进程的指针
 */
process *get_highest_process()
{
    int max_priority = 0;
    process_node *ptemp = process_ready->next;
    process *p = nullptr;
    int wait = 0;
    while (ptemp != nullptr)
    {
        if (ptemp->proc->priority > max_priority)
        {
            max_priority = ptemp->proc->priority;
            wait = ptemp->proc->wait;
            p = ptemp->proc;
        }
        else if (ptemp->proc->priority == max_priority)
        {
            if (ptemp->proc->wait >= wait)
            {
                max_priority = ptemp->proc->priority;
                wait = ptemp->proc->wait;
                p = ptemp->proc;
            }
        }

        ptemp = ptemp->next;
    }
    return p;
}

/**
 * @brief 根据rid获得资源的指针
 * @param {string} rid
 * @return {*}
 * 返回资源标识为rid的资源的指针
 */
resouce_node *get_resouce(string rid)
{
    resouce_node *rtemp = resouce_list->next;
    while (rtemp != nullptr)
    {
        if (rtemp->resc->rid == rid)
            return rtemp;
        rtemp = rtemp->next;
    }
    return nullptr;
}

/**
 * @brief 将就绪队列中所需资源为rid的进程从就绪队列转移到阻塞队列 
 * 将进程从就绪队列中去除 
 * 同时将进程的状态从就绪改为阻塞
 * @param {string} rid
 * @return {*}
 * 
 */
void readytoblock(string rid)
{
    process_node *temp = process_ready;
    process_node *ptemp = process_ready->next;
    while (ptemp != nullptr)
    {
        if (ptemp->proc->resouce == rid)
        {
            ptemp->proc->pstatus = "block";
            temp->next = ptemp->next;
            ptemp = ptemp->next;
        }
        else
        {
            temp = ptemp;
            ptemp = ptemp->next;
        }
    }
    return;
}

/**
 * @brief 将进程队列中所需资源为rid的进程放入就绪队列
 * 同时修改进程状态
 * @param {string} rid
 * @return {*}
 */
void blocktoready(string rid)
{
    process_node *ptemp = process_list->next;
    while (ptemp != nullptr)
    {
        if (ptemp->proc->resouce == rid)
        {
            ptemp->proc->pstatus = "ready";
            insert_process_ready(ptemp->proc);
        }
        ptemp = ptemp->next;
    }
    return;
}

/**
 * @brief 在进程队列中删除进程p
 * @param {process} *p
 * @return {*}
 */
void delete_process_list(process *p)
{
    process_node *temp = process_list;
    process_node *ptemp = process_list->next;
    while (ptemp != nullptr)
    {
        if (ptemp->proc == p)
        {
            temp->next = ptemp->next;
            ptemp = ptemp->next;
            return;
        }
        temp = ptemp;
        ptemp = ptemp->next;
    }
    cout << "delete_process_list error." << endl;
    return;
}

/**
 * @brief 为进程队列中除cpu正在运行中的进程外的其他进程的wait等待时间加一
 * @param {process} *cpu
 * @return {*}
 */
void waitplusone(process *cpu)
{
    process_node *ptemp = process_list->next;
    while (ptemp != nullptr)
    {
        if (ptemp->proc != cpu)
            ptemp->proc->wait++;
        ptemp = ptemp->next;
    }
}

/**
 * @brief 进程调度运行若干步数
 * @param {*}
 * @return {*}
 */
void runsteps()
{
    cout << "Input number of step you want to run: ";
    int step = 1;
    cin >> step;
    for (int i = 0; i < step; i++)
        run_one_step();

    show_process(process_list);
    return;
}

/**
 * @brief 进程调度与运行一次
 * @param {*}
 * @return {*}
 */
void run_one_step()
{
    if (cpu == nullptr)
    {
        if (process_ready->next == nullptr)
        {
            cout << "No process ready to cpu." << endl;
        }
        else
        {
            process *p = get_highest_process();
            if (!p->step)
            {
                cout << "Step Error." << endl;
                return;
            }
            p->step--;
            resouce_node *r = get_resouce(p->resouce);
            r->resc->rstatus = "allocated";
            readytoblock(p->resouce);
            cpu = p;
            p->pstatus = "running";
            p->wait = 0;
        }
    }
    else
    {

        if (cpu->step == 0)
        {
            delete_process_list(cpu);
        }
        resouce_node *r = get_resouce(cpu->resouce);
        r->resc->rstatus = "free";
        blocktoready(cpu->resouce);
        process *p = get_highest_process();
        if (p == nullptr)
        {
            cout << "No process ready to cpu." << endl;
            return;
        }
        if (!p->step)
        {
            cout << "Step Error." << endl;
            return;
        }
        p->step--;
        r = get_resouce(p->resouce);
        r->resc->rstatus = "allocated";
        readytoblock(p->resouce);
        cpu = p;
        p->pstatus = "running";
        p->wait = 0;
    }
    waitplusone(cpu);
    return;
}

int main()
{
    process_list = InitProcess(process_list);
    process_ready = InitProcess(process_ready);
    resouce_list = InitResouce(resouce_list);
    process_number = 0;
    resouce_number = 0;

    int choice;
    while (true)
    {
        cout << "1. create a process" << endl;
        cout << "2. create a resouce" << endl;
        cout << "3. delete a process" << endl;
        cout << "4. delete a resouce" << endl;
        cout << "5. show cpu" << endl;
        cout << "6. show all process" << endl;
        cout << "7. show ready process" << endl;
        cout << "8. run steps" << endl;
        cout << "0. exit" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Input choice number: ";
        cin >> choice;

        switch (choice)
        {
        case 0:
            exit(0);
            break;
        case 1:
            create_process();
            break;
        case 2:
            create_resouce();
            break;
        case 3:
            delete_process();
            break;
        case 4:
            delete_resouce();
            break;
        case 5:
            show_cpu();
            break;
        case 6:
            show_process(process_list);
            break;
        case 7:
            show_process(process_ready);
            break;
        case 8:
            runsteps();
            break;
        }
    }
    return 0;
}
