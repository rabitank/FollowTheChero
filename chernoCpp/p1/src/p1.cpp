// p1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "inc.h"

int g_sum=0; // 全局变量

//void p2_gsum();//前置声明,即使实现在另一个cpp文件能也可以使用,加入.h
void inter(int& value);

int main()
{
    std::cout << "==========part1==========" << std::endl;
    float a = 1.5, b;
    b = 3;
    //debug 内存查看:常用的X86结构是小端模式，高位保存在高位，低位保存在低位。这里左低右高，上低下高。
    //cc 表示未初始化, 00两个十六进制数表示一个字节
    //逐语句是真的逐语句,会在call函数时进入函数继续按行执行,而逐过程F10,在面对call函数时会直接执行到结果返回,并不会跳转到函数实现
    
    const float c = 300000000; //const 常变量,不能改变,只能在初始化时赋值,克服手贱的习惯;
    std::cout << "Hello World!\n"<<"\a \"cnm\" \r\n";
    std::cout << "\a\a\a\a\a\r\n"<<a<<b;
    int x = 5, y = 2;
    char z = 'a';
    double d = 1;
    std::cout << "z+d"<<z + d << "\r\n";
    // 整型时 / 求商 , %求余数(%运算只有整型)
    p2_gsum();
    std::cout << "g_sum=" << g_sum << std::endl;

    std::cout << "============part2==========" << std::endl;

    //数组
    int stu_nu[60];
    int num = 100;
    //int stu_nu2[num];//不行,num可变
    const int num2 = 100;
    int stu_nu3[num2];//可以,num2为常变量
    int* stu_nu4 = new int[num];//可以,指向一个新的动态数组
    //初始化
    int stu_nu5[] = { 2,3,4,0,0 };//2,3,4,0,0 等同于 stu_nu5[5]={2,3,4};

    //字符组
    char student[10];
    student[0] = 'h';
    //未初始化部分为\0(字符串结束表示符),有\0的可以看作字符串,当<<时默认作为字符串输出,没有\0将会随机访存直到遇到\0
    char student1[] = "i love c++";//最后还有一个隐藏字符 \0

    const char* ptr = nullptr;
    if (a == 3)
        std::cout << "just one line!" << std::endl;
    ///if 单语句就可以像上面那么写,指针,如果是nullptr 会被标识为false,你可以直接使用它作为判别式
    if (nullptr)
        std::cout << "ptr" << std::endl;
    else
        std::cout << "ptr is nullptr" << std::endl;

    std::cout << "==========part3==========" << std::endl;//指针;
    int var = 8;
    void* ptr = &var; //0并不指地址,它的意义是nullptr;void 指并未规定类型
    // 还可以用NULL,nullptr
    // &取地址,ptr就是一个十六进制的整数数字,表示地址
    double* ptr1 = (double*)&var;
    //对于不同数据类型的指针,它只是编译器为了规整的强制要求,并无不同,
    //(double*)强制转换为double类型指针,但内容上,所附地址没有实质性变化
    //*ptr = 16;报错
    //void 类型指针在逆向引用时并不知道其指向地址的数据类型,编译器不知道读取/写入几位
    //这时进行赋值就会报错
    *ptr1 = 16;
    //double规定了写入字节为16位,编译通过
    char* buffer = new char[8];
    //规定一个字节指针,new后面的告诉它有8个单位,并且在堆中,不会自动回收
    memset(buffer, 0, 8);//memset 在地址开头填充8个单位的0字节
    char** bufferptr = &buffer;
    //char类型指针(提供多少字节位信息)的指针

    std::cout << "==========part4==========" << std::endl;//引用;

    int wd = 5;
    int& ref = wd;// 引用等同于 int* ptr = &wd ; ref出现等同于 *ptr解引用
    //但是引用不会真的创建变量,它只是一种语法糖,这点不同于指针
    //引用没有预留改变指向的运算,它只能在初始化时确定引用一个已经存在变量,这点和指针不同;
    //相当于为这个变量取另一个名字,这是传递指向一个确定变量的好方法

    inter(wd);
    //part4 见下个工程了,讲到类了
}
void inter(int& value) 
{//对传递的变量创建一个引用叫value,基本上引用能干的事指针也能
    value++;

}



////////////////垃圾堆/////////////////////////

// ctrl+b 编译当前cpp文件 , ctrl + shift +B 编译整个解决方案;

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
