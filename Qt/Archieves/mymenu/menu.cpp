#include "menu.h"

menu::menu(QWidget *parent)
    : QWidget(parent)
{
    list=new QListWidget;//新建窗口list

    list->setIconSize(QSize(100,45));//设置项目（图片）大小
    list->setSpacing(12);//设置项目（图片）间距
    list->setViewMode(QListView::IconMode);//成行显示项目，注解显示在对应项目下面
    list->setMaximumWidth(120);//设置最大显示宽度
    list->setMovement(QListView::Static);//把图片设置为静态的不可拖拽的




    QListWidgetItem *djly=new QListWidgetItem;//添加图片项目进menu
    djly->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/djly.PNG"));//添加图片路径

    djly->setText(tr("剁椒鲈鱼"));//给项目加入注解

    QListWidgetItem *sbj=new QListWidgetItem;
    sbj->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/sbj.PNG"));
    sbj->setText(tr("三杯鸡"));

    QListWidgetItem *xglc=new QListWidgetItem;
    xglc->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/xglc.PNG"));
    xglc->setText(tr("鲜果龙船"));

    QListWidgetItem *srfssb=new QListWidgetItem;
    srfssb->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/srfssb.PNG"));
    srfssb->setText(tr("粉丝扇贝"));

    QListWidgetItem *zgdxj=new QListWidgetItem;
    zgdxj->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/zgdxj.PNG"));
    zgdxj->setText(tr("榛菇炖小鸡"));

    QListWidgetItem *zlhgr=new QListWidgetItem;
    zlhgr->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/zlhgr.PNG"));
    zlhgr->setText(tr("糟辣回锅肉"));

    QListWidgetItem *lkjc=new QListWidgetItem;
    lkjc->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/lkjc.PNG"));
    lkjc->setText(tr("辣烤鸡翅"));

    QListWidgetItem *dazj=new QListWidgetItem;
    dazj->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/dazj.PNG"));
    dazj->setText(tr("东安子鸡"));

    //QTextEdit *ordered=new QTextEdit;//创建用于显示已经点好的菜单目录


    list->addItem(djly);//将图片djly键入到list窗口
    list->addItem(sbj);
    list->addItem(xglc);
    list->addItem(srfssb);
    list->addItem(zgdxj);
    list->addItem(zlhgr);
    list->addItem(lkjc);
    list->addItem(dazj);


    QPushButton * clo=new QPushButton;//创建按钮
    connect(clo,SIGNAL(clicked()),this,SLOT(close()));
                                     //添加信号槽clo：按钮，
                                    //SIGNAL(clicked())：点击clo按钮发出信号，
                                   //this：本窗体接受信号，
                                  //SLOT(close()):调用函数close结束进程

    clo->setText(tr("关闭"));//按钮上显示文字：关闭
    //clo->resize(20,5);
    QPushButton * allok=new QPushButton;
    allok->setText(tr("确认点菜"));
    connect(allok,SIGNAL(clicked()),this,SLOT(sendorder()));


    infowidget=new QStackedWidget;//创建infowidget窗体
    wdjly=new pdjly;//创建实体分配空间
    wsbj=new psbj;
    wxglc=new pxglc;
    wsrfssb=new psrfssb;
    wzgdxj=new pzgdxj;
    wzlhgr=new pzlhgr;
    wlkjc=new plkjc;
    wdazj=new pdazj;

    infowidget->addWidget(wdjly);//把menu类中定义的项目添加进去
    infowidget->addWidget(wsbj);
    infowidget->addWidget(wxglc);
    infowidget->addWidget(wsrfssb);
    infowidget->addWidget(wzgdxj);
    infowidget->addWidget(wzlhgr);
    infowidget->addWidget(wlkjc);
    infowidget->addWidget(wdazj);

    connect(list,SIGNAL(currentRowChanged(int)),this,SLOT(pagechanged(int)));
    //connect(list,SIGNAL(currentRowChanged(int)),this,SLOT());


/*
    QVBoxLayout *vlayout=new QVBoxLayout(this);//新建布局管理器
    vlayout->addWidget(list);//将布局管理器键入到list窗口中
    vlayout->addWidget(ordered);//将文档查看器加入到布局管理器
    vlayout->addWidget(clo);//将clo添加到布局管理器中使用布局管理器管理
    this->setLayout(vlayout);//设置布局管理器的横纵向关系
*/

    QHBoxLayout *hlayout=new QHBoxLayout;//创建一个纵向布局管理器
    hlayout->addWidget(list);//添加
    hlayout->addWidget(infowidget);

    QGridLayout *mainlayout=new QGridLayout(this);//创建主布局管理器
    mainlayout->addLayout(hlayout,0,0,1,6);//将布局管理器的窗口加入进主布局管理器
    //mainlayout->addWidget(ordered,1,6,1,1);
    mainlayout->addWidget(allok,1,3,1,1);
    mainlayout->addWidget(clo,1,5,1,1);
    this->resize(800,600);//设置窗口大小




}

menu::~menu()
{
    ofstream ofile("C:/Qt/2010.04/qt/mymenu/menu.txt",ios::out);

    ofile.clear();

    ofile.close();

}

void menu::pagechanged(int index)//自定义的接受函数
{
    infowidget->setCurrentIndex(index);
   // infowidget->
    switch(index)
    {
    //infowidget->widget(index);

    case 0:
        ((pdjly *)infowidget->widget(index))->reflash();
        break;
    case 1:
        ((psbj *)infowidget->widget(index))->reflash();
        break;
    case 2:
        ((pxglc *)infowidget->widget(index))->reflash();
        break;
    case 3:
        ((psrfssb *)infowidget->widget(index))->reflash();
        break;
    case 4:
        ((pzgdxj *)infowidget->widget(index))->reflash();
        break;
    case 5:
        ((pzlhgr *)infowidget->widget(index))->reflash();
        break;
    case 6:
        ((plkjc *)infowidget->widget(index))->reflash();
        break;
    case 7:
        ((pdazj *)infowidget->widget(index))->reflash();
        break;
    }
}

void menu::sendorder()
{
    char temp;


    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    port=new Win_QextSerialPort("COM4");

    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);



    port->open(QIODevice::ReadWrite);


   ifile.seekg(0);

    while(!ifile.eof())
    {
          ifile>>temp;

          if(temp!='a')
          {

              while(!port->isWritable())
                {;}

              port->putChar('0');
              while(!port->isWritable())
                {;}
              port->putChar('0');
              while(!port->isWritable())
                {;}
              port->putChar('1');

              while(!port->isWritable())
                {;}
              port->putChar('0');
              while(!port->isWritable())
                {;}
              port->putChar('0');
              while(!port->isWritable())
                {;}
              port->putChar(temp);

              temp='a';
          }
    }

    port->close();

}


