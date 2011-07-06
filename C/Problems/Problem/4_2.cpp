/* 请添加函数复制构造函数、重载赋值运算符、重载输入运算符、重载输出运算符使 main 函数能完成相应的功能。*/

# include <iostream>

class Class
{
    public:
        Class();
        /* code in below area */
        Class(Class const & cls);
        Class & operator = (Class const &cls);
        /* code in above area */
        int i;
};

/* code in below area */
Class::Class()
{
    //Do nothing
}
Class & Class::operator=(Class const& cls)
{
    this->i=cls.i;
    return *this;
}
Class::Class(Class const & cls)
{
    this->i=cls.i;
}
std::istream & operator >> (std::istream &in, Class &cls1)
{
    in>>cls1.i;
    return in;
}
std::ostream & operator << (std::ostream &out,Class &cls2)
{
    out<<cls2.i;
    return out;
}
/* code in above area */

/* 测试函数 */
int main()
{
    Class temp1, temp2;

    std::cout << "set value" << std::endl;
    // 输入temp1
    std::cin >> temp1;
    // 赋值
    temp2 = temp1;
    // 输出 temp1
    std::cout << "temp1: " << temp1 << std::endl;
    // 输出 temp2
    std::cout << "temp2: " << temp2 << std::endl;

    Class temp3(temp1);
    // 输出 temp3
    std::cout << "temp3: " << temp3 << std::endl;

    return 0;
}
