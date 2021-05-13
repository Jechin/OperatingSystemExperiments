#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
using namespace std;

#define PMAX 10 //max number of process
#define RMAX 10 //max number of resouce

int pnum, rnum;
int pid;

int Available[RMAX];
int Allocated[PMAX][RMAX];
int Need[PMAX][RMAX];
int MAX[PMAX][RMAX];
int Station[PMAX];
int Safe[PMAX];
int Work[RMAX];
int Request[RMAX];

void Initial();
void Input();
void Show();
int check();
int check_process(int p);
void allo();
void menu();

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
        case 0:
            exit(0);
        case 1:
            Input();
            break;
        case 2:
            allo();
            break;
        case 3:
            Show();
            break;
        case 4:
            check();
            break;
        }
    }
    return 0;
}

/**
 * @brief 初始化函数
 * @param {*}
 * @return {*}
 */
void Initial()
{
    memset(Available, 0, RMAX);
    memset(Allocated, 0, sizeof(Allocated));
    memset(Need, 0, PMAX * RMAX);
    memset(MAX, 0, PMAX * RMAX);
    memset(Station, 0, PMAX);
    memset(Safe, 0, PMAX);
    cout << "Initial Successfully." << endl;
    return;
}

/**
 * @brief 输入函数
 * 输入资源可用矩阵
 * 输入资源已分配矩阵（可选）
 * 输入进程需求矩阵
 * @param {*}
 * @return {*}
 */
void Input()
{
    cout << "Input number of process: ";
    cin >> pnum;
    cout << "Input number of resouce: ";
    cin >> rnum;

    int p, r;
    cout << "Input available number of each resouce: " << endl;
    for (r = 0; r < rnum; r++)
    {
        cout << "  resouce " << r << ": ";
        cin >> Available[r];
    }

    cout << "Input allocation matrix?(y/n): ";
    char choice;
    while (cin >> choice)
    {
        if (choice == 'y')
        {
            cout << "Input each resouce allocated for each process:" << endl;
            for (p = 0; p < pnum; p++)
            {
                cout << "  process " << p << ": " << endl;
                for (r = 0; r < rnum; r++)
                {
                    cout << "    resouce " << r << ": ";
                    cin >> Allocated[p][r];
                }
            }
            break;
        }
        if (choice == 'n')
            break;
        cout << "Input error." << endl;
    }

    cout << "Input max number of each resouce needed for each process: " << endl;
    for (p = 0; p < pnum; p++)
    {
        cout << "  process " << p << ": " << endl;
        for (r = 0; r < rnum; r++)
        {
            cout << "    resouce " << r << ": ";
            cin >> MAX[p][r];
            Need[p][r] = MAX[p][r] - Allocated[p][r];
        }
    }
    Show();
}

/**
 * @brief 输出各矩阵信息
 * @param {*}
 * @return {*}
 */
void Show()
{
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "pid"
         << "     "
         << "allocated"
         << "     "
         << "need"
         << "     "
         << "station"
         << endl;
    int p, r;
    for (p = 0; p < pnum; p++)
    {
        cout << "P" << p << "      ";
        for (r = 0; r < rnum; r++)
            cout << setw(2) << Allocated[p][r];
        cout << "     ";
        for (r = 0; r < rnum; r++)
            cout << setw(2) << Need[p][r];
        cout << "     ";
        if (Station[p] == 0)
            cout << "wait" << endl;
        else
            cout << "finish" << endl;
    }
    cout << "-----------------------------------------------------------------------------" << endl;
}

/**
 * @brief 检查当前资源矩阵下是否存在安全序列
 * @param {*}
 * @return {*}
 */
int check()
{
    memcpy(Work, Available, RMAX);
    int result = 0;
    int num = 0;
    int success = 0;
    int temp = 0;
    int p, r;
    for (p = 0; p < pnum; p++)
    {
        temp = 0;
        if (!Station[p] && check_process(p))
        {
            Safe[num++] = p;
            for (r = 0; r < rnum; r++)
                Work[r] += Allocated[p][r];
            Station[p] = 1;
        }
        if (p == pnum - 1) //检查该轮循环结束进程数量是否增加
        {
            for (int i = 0; i < pnum; i++)
                temp += Station[p];
            if (success != temp)
            {
                success = temp;
                p = -1;
            }
        }
    }
    if (success == pnum)
    {
        cout << "Safe!" << endl;
        cout << "Safe series: ";
        for (p = 0; p < pnum; p++)
            cout << "P" << Safe[p] << " ";
        cout << endl;
        memset(Station, 0, PMAX);
        return 1;
    }
    else
    {
        memset(Safe, 0, PMAX);
        memset(Station, 0, PMAX);
        cout << "Unsafe!" << endl;
        return 0;
    }
}

/**
 * @brief 检查进程p是否满足资源份分配
 * @param {int} p
 * @return {*}
 */
int check_process(int p)
{
    int r;
    for (r = 0; r < rnum; r++)
    {
        if (Need[p][r] > Work[r])
            return 0;
    }
    return 1;
}

/**
 * @brief 尝试分配资源
 * @param {*}
 * @return {*}
 */
void allo()
{
    memset(Request, 0, RMAX);
    cout << "Input pid of process you want to allocate: ";
    cin >> pid;
    if (pid >= pnum)
    {
        cout << "No such process. Input pid between 0 to " << pnum - 1 << endl;
        return;
    }
    cout << "Input number of each resouce you want to allocated:" << endl;
    int r;
    for (r = 0; r < rnum; r++)
    {
        cout << "  resouce " << r << ": ";
        cin >> Request[r];
        if (Request[r] > Need[pid][r])
        {
            cout << "Request is over need." << endl;
            r--;
            continue;
        }
        if (Request[r] > Available[r])
        {
            cout << "Request is over available." << endl;
            r--;
            continue;
        }
        Allocated[pid][r] += Request[r];
        Need[pid][r] -= Request[r];
        Available[r] -= Request[r];
    }

    if (check())
    {
        cout << "Request Successfully." << endl;
    }
    else
    {
        cout << "Request Failed." << endl;
        for (r = 0; r < rnum; r++)
        {
            Allocated[pid][r] -= Request[r];
            Need[pid][r] += Request[r];
            Available[r] += Request[r];
        }
    }

    return;
}

void menu()
{
    cout << "1. input" << endl;
    cout << "2. allocate resources" << endl;
    cout << "3. print allocate answer" << endl;
    cout << "4. safe check" << endl;
    cout << "0. exit" << endl;
    cout << "Input your choice: ";
}