#include "mypage.h"


pdjly::pdjly(QWidget *parent)
    : QWidget(parent)
{


    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("剁椒  鲈鱼"));
    styleline=new QLineEdit(tr("川菜"));
    specline=new QLineEdit(tr("色泽鲜艳，美味可口，驱寒祛湿"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("处理干净的鲈鱼一条"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("新鲜的花椒一束 、盐、葱叶、姜片"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("米酒、香油、米醋、番茄酱"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("白糖、蒜末、生抽、生粉"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、鱼内外抹盐，肚皮里填葱叶、姜片，腌半小时"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、将鱼腹刨开，去骨，摊平鱼肉，两边各切成数条，淋两勺米酒、几滴香油、两滴米醋，加半杯水，装入微波盘"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、家里的微波炉刚好极限尺寸可以斜塞下这个椭圆盘，因为不需要旋转，就能加热，所以只要能塞进去即可。 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.时间到，用微波盘内蒸鱼流出的鱼汁，加番茄酱、白糖、蒜末、生抽、生粉、米酒、盐，勾成芡汁"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、做好的芡汁均匀的浇在清蒸好的鱼上即可。"));
      QListWidgetItem *it6=new QListWidgetItem(introline);
      it6->setText(tr("6、点缀上熟玉米、熟青豆、剁椒、罗勒叶，味道很鲜美。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/djly.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

     // QTextEdit *ordered=new QTextEdit;//创建用于显示已经点好的菜单目录
      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器

    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);



    this->setLayout(glayout);//将布局管理器添加进窗口
 //------------------------------------
    reflash();
//----------------------------------


}

void pdjly::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<1<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}


//--------------------------
void pdjly::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------







psbj::psbj(QWidget *parent)
    : QWidget(parent)
{
    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("三 杯 鸡"));
    styleline=new QLineEdit(tr("山东"));
     specline=new QLineEdit(tr("香气袭人，口感极佳"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("鸡翅跟"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("香油，老抽，酒"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("葱，姜，蒜"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("糖，蚝油，盐，五香粉，蜂蜜"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、鸡翅洗净后，开水烫煮鸡肉半分钟，去血末"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、用刀在鸡肉上口，用姜茸、葱段、料酒，蚝油，五香粉揉匀，腌一小时"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、腌好后，擦净鸡肉上的姜茸之类,要清理干净 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.锅里倒入香油，放入葱段、姜片、蒜瓣翻炒，出香后放入鸡块炒至鸡肉表面略黄"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、倒入酱油上色，再倒入料酒，加入蜂蜜"));
      QListWidgetItem *it6=new QListWidgetItem(introline);
      it6->setText(tr("6、中小火焖至鸡肉酥软后，大火收汁，放入葱段再翻炒几下即可"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/sbj.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(speclabel);
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);


    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void psbj::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<2<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void psbj::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------





pxglc::pxglc(QWidget *parent)
    : QWidget(parent)
{
    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("鲜果  龙船"));
    styleline=new QLineEdit(tr("福建"));
     specline=new QLineEdit(tr("嫩、鲜、滑、有营养"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("桂鱼"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("鲜奶、高汤"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("盐、味精"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("黄酒、水菱粉"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、将桂鱼去内脏"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、去骨、批成片"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、卷成鱼卷，上浆 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.将鱼卷放入油锅划一下，捞起"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、锅内加入高汤、调料"));
      QListWidgetItem *it6=new QListWidgetItem(introline);
      it6->setText(tr("6、鱼卷烧开后，再 加入鲜奶，勾上菱粉即成。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/xglc.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);



    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void pxglc::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<3<<endl;
    ofile.close();
    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void pxglc::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------





psrfssb::psrfssb(QWidget *parent)
    : QWidget(parent)
{

    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("蒜蓉粉丝扇贝"));
    styleline=new QLineEdit(tr("广东"));
    specline=new QLineEdit(tr("色泽鲜嫩，营养美味，口感极佳"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("新鲜扇贝"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("大蒜、香葱"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("色拉油、盐"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("鸡粉、龙口粉丝"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、清洗干净将贝肉完全剔除出来"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、用盐水浸泡两三分钟，用筷子顺时针旋转贝肉，这样做的目的是使贝肉表面的泥沙沉入碗底"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、把贝肉放入碗中，用手搓洗，最后，再用清水洗一遍即可。 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.龙口粉丝浸泡在凉水中半个小时，备用"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、大蒜剥皮，用大蒜器将蒜打碎，香葱切葱花，备用。"));
      QListWidgetItem *it6=new QListWidgetItem(introline);
      it6->setText(tr("6、将蒜蓉倒入油锅，迅速翻炒，至蒜蓉淡黄色，倒入一个大碗中，备用。"));
      QListWidgetItem *it7=new QListWidgetItem(introline);
      it7->setText(tr("7、，将蒜蓉中加入盐和鸡粉调味。"));
      QListWidgetItem *it8=new QListWidgetItem(introline);
      it8->setText(tr("8、将粉丝呈鸟巢状分别摆入扇贝壳中，然后将贝肉在放在粉丝上。"));
      QListWidgetItem *it9=new QListWidgetItem(introline);
      it9->setText(tr("9、将蒜蓉连同色拉油一起涂抹在扇贝与粉丝上。"));
      QListWidgetItem *it10=new QListWidgetItem(introline);
      it10->setText(tr("10、蒸锅已开，我们将扇贝整齐地摆在盘中，上蒸锅，大火三分钟，即可。"));
      QListWidgetItem *it11=new QListWidgetItem(introline);
      it11->setText(tr("11、将热油再依次撒在香葱葱花上即成。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/srfssb.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);



    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void psrfssb::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<4<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void psrfssb::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------





pzgdxj::pzgdxj(QWidget *parent)
    : QWidget(parent)
{
    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("榛菇 炖 小鸡"));
    styleline=new QLineEdit(tr("东北"));
     specline=new QLineEdit(tr("简单易做，品味纯正"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("嫩公鸡一只"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("榛菇"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("干红辣椒"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("八角、葱姜蒜"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、榛蘑淘洗干净，用温水泡发30分钟，沥干待用"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、鸡洗净，剁成小块"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、鸡块在开水中焯3分钟，捞出用热水冲去浮沫，沥干水分备用。 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.冷锅热油，油热后下入鸡块大火爆炒5分钟"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、把葱姜蒜八角和红椒用锅底的热油爆香，然后用大火翻炒一分钟。"));
      QListWidgetItem *it6=new QListWidgetItem(introline);
      it6->setText(tr("6、烹入料酒，调入生抽，加糖，加热水大火烧开，转中火炖20分钟。"));
      QListWidgetItem *it7=new QListWidgetItem(introline);
      it7->setText(tr("7、添加泡发好的榛菇，中小火继炖20分钟，添加适量盐调味。"));
      QListWidgetItem *it8=new QListWidgetItem(introline);
      it8->setText(tr("8、大火收汁，出锅前撒入葱花即可。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/zgdxj.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);



    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void pzgdxj::ok_clicked()
{

    ordered->clear();
    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<5<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void pzgdxj::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------





pzlhgr::pzlhgr(QWidget *parent)
    : QWidget(parent)
{
    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr(" 糟辣 回 锅肉"));
    styleline=new QLineEdit(tr("云南"));
     specline=new QLineEdit(tr("香味扑鼻百米可闻，辣爽美味"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("猪五花肉"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("青蒜（蒜苗）、老姜、老姜丝"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("白砂糖、生抽、鸡精 "));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("葱白丝、糟辣椒酱、辣椒水豆豉 "));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、将整块五花肉洗净，加老姜片一起在锅中小火煨煮，捞出沥干水分晾凉"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、青蒜（蒜苗）切成3cm长的段。晾凉的五花肉切成尽量完整的薄片"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、中火加热净炒锅，将五花肉片入锅，煸出油。 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4、将肉片拨至炒锅的一边，改大火放入糟辣椒酱和辣椒水豆豉，炒匀后将老姜丝和葱白丝爆香"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、调入白砂糖，将青蒜（蒜苗）段放入同炒3分钟，最后调入生抽和鸡精即可。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/zlhgr.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);


    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void pzlhgr::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<6<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void pzlhgr::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------





plkjc::plkjc(QWidget *parent)
    : QWidget(parent)
{
    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("辣烤  鸡翅"));
    styleline=new QLineEdit(tr("京普"));
     specline=new QLineEdit(tr("正宗醇香，口感极佳"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("鸡翅"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("盐、红糖、蒜泥"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("蜂蜜、酱油、料酒"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("辣椒粉、花椒粉、五香粉"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、，将所有调料放入鸡肉，和匀，让鸡翅均匀地沾到调料。"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、将鸡翅密封放入冰箱冷藏保存12个小时。"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、将腌好的鸡翅均匀放入烤盘，刷上调料，用上火烤10分钟。 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.取出鸡翅，翻面，刷上调料，再放入烤箱烤10分钟，鸡翅表面呈金黄色即可。"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、食用时撒上辣椒、花椒粉。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/lkjc.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));

      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);



    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void plkjc::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<7<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void plkjc::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------





pdazj::pdazj(QWidget *parent)
    : QWidget(parent)
{
    namelabel=new QLabel(tr("菜名："));
    stylelabel=new QLabel(tr("菜系："));
    speclabel=new QLabel(tr("特点"));
    conslabel=new QLabel(tr("配料："));
    introlabel=new QLabel(tr("做法简介："));

    nameline=new QLineEdit(tr("东安  子鸡"));
    styleline=new QLineEdit(tr("湖南"));
     specline=new QLineEdit(tr("鸡肉肥嫩，味道酸辣鲜香"));

    consline=new QListWidget;
    consline->setSpacing(3);
    QListWidgetItem *consitem=new QListWidgetItem(consline);
    consitem->setText(tr("肥嫩子母鸡"));
    QListWidgetItem *consitem3=new QListWidgetItem(consline);
    consitem3->setText(tr("小鲜红椒、猪油、料酒"));
    QListWidgetItem *consitem4=new QListWidgetItem(consline);
    consitem4->setText(tr("盐、米醋、味精、花椒粉"));
    QListWidgetItem *consitem2=new QListWidgetItem(consline);
    consitem2->setText(tr("葱、姜、湿淀粉、香油、鸡汤"));




      introline=new QListWidget;
      introline->setSpacing(3);
      QListWidgetItem *it1=new QListWidgetItem(introline);
      it1->setText(tr("1、宰鸡去毛，开膛去内脏洗净，放入汤锅白煮七成熟，鸡肉内部还有血时捞出，稍凉去净骨"));
      QListWidgetItem *it2=new QListWidgetItem(introline);
      it2->setText(tr("2、剁成5厘米长2厘米宽的条"));
      QListWidgetItem *it3=new QListWidgetItem(introline);
      it3->setText(tr("3、小红椒去蒂去籽洗净，与姜切成细丝，葱切成3厘米长的段。 "));
      QListWidgetItem *it4=new QListWidgetItem(introline);
      it4->setText(tr("4.将猪油烧到六成热，下入姜丝、红椒丝煸炒出香辣味"));
      QListWidgetItem *it5=new QListWidgetItem(introline);
      it5->setText(tr("5、下鸡块炒一下，烹料酒，放入盐、醋和汤，使香辣味、盐醋味渗透入鸡肉内。"));
      QListWidgetItem *it6=new QListWidgetItem(introline);
      it6->setText(tr("6、加入葱段，收汁，用湿淀粉勾芡，淋香油，装盘即成。"));

      QListWidget *pic=new QListWidget;
      QListWidgetItem *picitem=new QListWidgetItem(pic);
      picitem->setIcon(QIcon("C:/Qt/2010.04/qt/mymenu/dazj.PNG"));
      pic->setIconSize(QSize(470,370));

      QPushButton *ok=new QPushButton;
      ok->setText(tr("点菜"));
      connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));
      ordered=new QListWidget;//创建用于显示已经点好的菜单目录


    QGridLayout *glayout=new QGridLayout(this);//创建一个纵向的布局管理器
    //glayout->setSpacing(12);

    glayout->addWidget(namelabel,0,0);//将项目namelabel添加进布局管理器
    glayout->addWidget(nameline,0,1);     //数字参数横坐标，纵坐标，占用行格数，占用列格数
    glayout->addWidget(stylelabel,1,0);
    glayout->addWidget(styleline,1,1);
    glayout->addWidget(speclabel,2,0);
    glayout->addWidget(specline,2,1);
    //glayout->addWidget(packagegroup,2,0,2,2);
    glayout->addWidget(conslabel,3,0);
    glayout->addWidget(consline,3,1);

    glayout->addWidget(pic,0,3,4,4);

    glayout->addWidget(introlabel,4,0);
    glayout->addWidget(introline,4,1,4,4);
    glayout->addWidget(ok,8,5,1,1);
    glayout->addWidget(ordered,9,1,1,5);



    this->setLayout(glayout);//将布局管理器添加进窗口
    reflash();
}

void pdazj::ok_clicked()
{
    ordered->clear();

    ofstream ofile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::out|ios::app);
    ofile<<8<<endl;
    ofile.close();

    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();
}
//--------------------------
void pdazj::reflash()
{
    ordered->clear();


    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("1.剁椒鲈鱼"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("2.三杯鸡"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("3.鲜果龙船"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("4.蒜蓉粉丝扇贝"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("5.榛菇炖小鸡"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.糟辣回锅肉"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.辣烤鸡翅"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.东安子鸡"));
        }
    }
    ifile.close();

    /*
    djly
    sbj
    xglc
    srfssb
    zgdxj
    zlhgr
    lkjc
    dazj
    */
}

//-----------------------------
