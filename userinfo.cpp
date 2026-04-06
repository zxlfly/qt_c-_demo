#include "userinfo.h"
#include "ui_userinfo.h"

UserInfo::UserInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInfo)
{
    ui->setupUi(this);
}

UserInfo::~UserInfo()
{
    delete ui;
}
