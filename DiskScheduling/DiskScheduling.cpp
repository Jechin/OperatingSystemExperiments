#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <math.h>
using namespace std;

#define MAXTrack 50
#define MAXLoop 20

const int MaxNum = 200;
int Loop;
int TrackNum;
int Algorithm;
int Track[MAXTrack + 1];
int Order[MAXTrack + 1];
int Fin[MAXTrack + 1];
int Distance[MAXTrack + 1];
double AvMove[MAXLoop + 1];

void Menu();
void Input();
void Initial();
void FIFO();
void SSTF();
void SCAN();
void ShowOrder();
int NextSSTF(int now);
int SeletWay(int now);

/**
 * @brief 显示菜单函数，选择调度算法
 * @param {*}
 * @return {*}
 */
void Menu()
{
    cout << "1. FIFO" << endl;
    cout << "2. SSTF" << endl;
    cout << "3. SCAN" << endl;
    cout << "0. exit" << endl;
    cout << "Input your choice: ";
    cin >> Algorithm;
    return;
}

/**
 * @brief 输入信息
 * TrackNum 设定需要扫描的磁道数
 * Loop 设定测试循环次数
 * @param {*}
 * @return {*}
 */
void Input()
{
    do
    {
        cout << "Input number of track (no more than 50): ";
        cin >> TrackNum;
        if (TrackNum < 0 || TrackNum > 50)
            cout << "Invalid Input." << endl;
    } while (TrackNum < 0 || TrackNum > 50);

    do
    {
        cout << "Input loop count (no more than 20): ";
        cin >> Loop;
        if (Loop < 0 || Loop > 50)
            cout << "Invalid Input." << endl;
    } while (Loop < 0 || Loop > 50);

    return;
}

/**
 * @brief 初始化并随机产生需要扫描的磁道
 * @param {*}
 * @return {*}
 */
void Initial()
{
    memset(Track, 0, MAXTrack + 1);
    memset(Order, 0, MAXTrack + 1);
    memset(Fin, 0, MAXTrack + 1);
    memset(AvMove, 0, MAXLoop + 1);
    memset(Distance, 0, MAXTrack + 1);

    int i;
    cout << "Randomly generate tracks: " << endl;
    for (i = 0; i < TrackNum; i++)
        Track[i] = rand() % MaxNum + 1;
    for (i = 0; i < TrackNum; i++)
    {
        if (i % 10 == 0)
            cout << "  ";
        cout << Track[i] << " ";
        if (i % 10 == 9)
            cout << endl;
    }
    return;
}

/**
 * @brief 输出扫描磁道序列
 * @param {*}
 * @return {*}
 */
void ShowOrder()
{
    cout << "Start From Track Number: " << Order[0] << endl;
    cout << setw(10) << "Next Track" << setw(18) << "Move Distance" << endl;
    int i;
    for (i = 1; i < TrackNum; i++)
        cout << setw(10) << Order[i] << setw(18) << Distance[i] << endl;
    return;
}

/**
 * @brief 先来先服务扫描磁道
 * @param {*}
 * @return {*}
 */
void FIFO()
{
    Input();
    int i, j;
    double Average = 0;
    int sum;
    for (i = 0; i < Loop; i++)
    {
        Initial();
        Order[0] = Track[0];
        sum = 0;
        for (j = 1; j < TrackNum; j++)
        {
            Order[j] = Track[j];
            Distance[j] = abs(Order[j] - Order[j - 1]);
            sum += Distance[j];
        }
        ShowOrder();
        printf("      Average Move Distance is: %.2f\n", (double)sum / TrackNum);
        Average += (double)sum / TrackNum;
        cout << endl;
    }
    printf("Average Move Distance of FIFO is: %.2f\n", (double)Average / Loop);
    return;
}

/**
 * @brief 计算磁头下一跳
 * @param {int} now 当前磁头位置
 * @return {*}
 * 返回磁头下一个位置
 * 全部扫描完成 返回-1
 */
int NextSSTF(int now)
{
    int closest = MaxNum + 1;
    int NextNumber = -1;
    for (int i = 0; i < TrackNum; i++)
    {
        if (!Fin[i])
        {
            if (abs(Track[i] - now) < closest)
            {
                closest = abs(Track[i] - now);
                NextNumber = i;
            }
        }
    }
    return NextNumber;
}

/**
 * @brief 最短寻道时间优先扫描磁道
 * @param {*}
 * @return {*}
 */
void SSTF()
{
    Input();
    int i, j, k;
    double Average = 0;
    int sum;
    int next;
    for (i = 0; i < Loop; i++)
    {
        Initial();
        Order[0] = Track[0];
        Fin[0] = 1;
        sum = 0;
        for (j = 1; j < TrackNum; j++)
        {
            next = NextSSTF(Order[j - 1]);
            if (next == -1)
            {
                cout << "Error" << endl;
                break;
            }
            Order[j] = Track[next];
            Fin[next] = 1;
            Distance[j] = abs(Order[j] - Order[j - 1]);
            sum += Distance[j];
        }
        ShowOrder();
        printf("      Average Move Distance is: %.2f\n", (double)sum / TrackNum);
        Average += (double)sum / TrackNum;
        cout << endl;
    }
    printf("Average Move Distance of SSTF is: %.2f\n", (double)Average / Loop);
    return;
}

/**
 * @brief 对需要扫描的磁道序列进行排序
 * @param {*}
 * @return {*}
 * 返回磁头当前位置
 */
int SortSCAN()
{
    int nowplace = Track[0];
    for (int i = 0; i < TrackNum - 1; i++)
    {
        for (int j = i + 1; j < TrackNum; j++)
        {
            if (Track[i] > Track[j])
                swap(Track[i], Track[j]);
        }
    }
    for (int i = 0; i < TrackNum; i++)
    {
        if (nowplace == Track[i])
            return i;
    }
    return -1;
}

/**
 * @brief 在电梯调度中选择扫描方向
 * @param {int} now
 * @return {*}
 * 1--先向上扫描
 * -1--先向下扫描
 */
int SeletWay(int now)
{
    if (now == 0)
        return 1;
    if (now == TrackNum - 1)
        return -1;
    if (abs(Track[now + 1] - Track[now]) < abs(Track[now] - Track[now - 1]))
        return 1;
    else
        return -1;
}

/**
 * @brief 电梯调度扫描磁道
 * @param {*}
 * @return {*}
 */
void SCAN()
{
    Input();
    int i, j, k;
    double Average = 0;
    int sum;
    int next;
    int way;
    int start;
    for (i = 0; i < Loop; i++)
    {
        Initial();
        next = SortSCAN();
        if (next == -1)
        {
            cout << "Error" << endl;
            continue;
        }
        start = next;
        Order[0] = Track[next];
        Fin[next] = 1;
        sum = 0;
        way = SeletWay(next);
        if (way == 1)
            next = start + 1;
        else if (way == -1)
            next = start - 1;
        for (j = 1; j < TrackNum; j++)
        {
            if (next > start)
            {
                if (next == TrackNum - 1)
                    next = start - 1;
                else
                    next++;
            }
            else
            {
                if (next == 0)
                    next = start + 1;
                else
                    next--;
            }
            Order[j] = Track[next];
            Fin[next] = 1;
            Distance[j] = abs(Order[j] - Order[j - 1]);
            sum += Distance[j];
        }

        ShowOrder();
        printf("      Average Move Distance is: %.2f\n", (double)sum / TrackNum);
        Average += (double)sum / TrackNum;
        cout << endl;
    }
    printf("Average Move Distance of SSTF is: %.2f\n", (double)Average / Loop);
    return;
}

int main()
{
    srand(time(0));
    while (true)
    {
        Menu();
        switch (Algorithm)
        {
        case 0:
            exit(0);
        case 1:
            FIFO();
            break;
        case 2:
            SSTF();
            break;
        case 3:
            SCAN();
            break;
        default:
            break;
        }
    }
    return 0;
}