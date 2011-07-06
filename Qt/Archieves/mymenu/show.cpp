void show(QWiget *a)
{
    int temp;
    ifstream ifile("C:\\Qt\\2010.04\\qt\\mymenu\\menu.txt",ios::in);

    while(!ifile.eof())
    {
        temp=0;
        ifile>>temp;

        if(temp==1)
        {
            QListWidgetItem *item=new QListWidgetItem(a.ordered);
            item->setText(tr("1.¶ç½·öÔÓã"));
        }

        else if(temp==2)
        {
            QListWidgetItem *item=new QListWidgetItem(a.ordered);
            item->setText(tr("2.Èý±­¼¦"));
        }

        else if(temp==3)
        {
            QListWidgetItem *item=new QListWidgetItem(a.ordered);
            item->setText(tr("3.ÏÊ¹ûÁú´¬"));

        }

        else if(temp==4)
        {
            QListWidgetItem *item=new QListWidgetItem(a.ordered);
            item->setText(tr("4.ËâÈØ·ÛË¿ÉÈ±´"));
        }

        else if(temp==5)
        {
            QListWidgetItem *item=new QListWidgetItem(a.ordered);
            item->setText(tr("5.é»¹½ìÀÐ¡¼¦"));
        }

        else if (temp==6)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("6.ÔãÀ±»Ø¹øÈâ"));
        }

        else if(temp==7)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("7.À±¿¾¼¦³á"));
        }

        else if(temp==8)
        {
            QListWidgetItem *item=new QListWidgetItem(ordered);
            item->setText(tr("8.¶«°²×Ó¼¦"));
        }
    }
    ifile.close();
}
