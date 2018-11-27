#include "audioeditor.h"
#include "ui_audioeditor.h"

AudioEditor::AudioEditor(QWidget *parent) :
    TabPage(parent),
    ui(new Ui::AudioEditor)
{
    ui->setupUi(this);

    ui->SpectrumTabel->setEditTriggers(QTableView::NoEditTriggers);
    ui->SpectrumTabel->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->SpectrumTabel->horizontalHeader()->setStretchLastSection(true);  //列宽度自适应
    ui->SpectrumTabel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//    ui->SpectrumTabel->setMinimumHeight(this->rect().height()/2);

    ui->AudioListTabel->setEditTriggers(QTableView::NoEditTriggers);
    ui->AudioListTabel->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->AudioListTabel->horizontalHeader()->setStretchLastSection(true);  //列宽度自适应
    ui->AudioListTabel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->AudioListTabel->setColumnCount(2);
    connect(ui->AudioListTabel,SIGNAL(cellDoubleClicked(int, int)),this,SLOT(slot_AudioListTabelDoubleClicked(int, int)));

    ui->RecordSetGroupbox->setHidden(true);
}

AudioEditor::~AudioEditor()
{
    delete ui;
}

void AudioEditor::RefreshAudioListTabel()
{
    ui->AudioListTabel->clear();
    quint8 num = m_MemberInfo.count();
    ui->AudioListTabel->setRowCount(num);
    if(num > 0)
    {
        for(int i=0;i<num;i++)
        {
            Tree_Member_Info member = m_MemberInfo.at(i);
            QTableWidgetItem *item = new QTableWidgetItem(0);
            switch(member.mState)
            {
                case Wait:
                    item->setIcon(QIcon(":/Menu/rc/Menu/pass.png"));
                    break;
                case Busy:
                    item->setIcon(QIcon(":/Menu/rc/Menu/debug.png"));
                    break;
                case Err:
                    item->setIcon(QIcon(":/Menu/rc/Menu/delete.png"));
                    break;
                case Finished:
                    item->setIcon(QIcon(":/Menu/rc/Menu/success.png"));
                    break;
            }
            item->setText(member.mAudioInfo.mName);
            ui->AudioListTabel->setItem(i,0,item);
            QProgressBar *tmpProgressBar = new QProgressBar(this);
            tmpProgressBar->setMaximumHeight(20);
            if(member.mProgress >= 100)
                member.mProgress = 100;
            tmpProgressBar->setValue(member.mProgress);
            ui->AudioListTabel->setCellWidget(i,1,tmpProgressBar);
        }
    }
}

void AudioEditor::RefreshSpectrumTabel(int pDex)
{
    quint8 curSpectrumCnt = ui->SpectrumTabel->rowCount();
    quint8 updateSpectrumCnt = m_MemberInfo.at(pDex).mAudioInfo.mSpectrumCnt;
    if(curSpectrumCnt == updateSpectrumCnt)
        return;
    ui->SpectrumTabel->setRowCount(updateSpectrumCnt);
    for(int i=curSpectrumCnt;i<updateSpectrumCnt;i++)
    {
        WaveForm *waveForm = new WaveForm(0);
        waveForm->setMinimumHeight(180);
        ui->SpectrumTabel->setRowHeight(i, 200);
        ui->SpectrumTabel->setCellWidget(i,0,waveForm);
    }
}

void AudioEditor::slot_AudioListTabelDoubleClicked(int row, int column)
{
    RefreshSpectrumTabel(row);
    if(ui->SpectrumTabel->rowCount()==0)
        return;
    WaveForm *waveForm = (WaveForm *)(ui->SpectrumTabel->cellWidget(0, 0));
    waveForm->PlayFile(m_MemberInfo.at(row).mAudioInfo.mPath);
}

void AudioEditor::on_AudioAdd_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(
                                        this,
                                        "Select one or more files to open",
                                        QDir::currentPath(),
                                        "Audio(*.wav *.mp3);;Video(*.avi *.mp4 *.flv *.rmvb);;All(*)");
    if(list.count() == 0)
        return;
    for(int i=0;i<list.count();i++)
    {
        Tree_Member_Info member;
        member.mAudioInfo.mPath = list.at(i);
        member.mAudioInfo.mName = getFileName(list.at(i));
        member.mAudioInfo.mSize = 0;
        member.mAudioInfo.mLenth = 0;
        member.mAudioInfo.mSpectrumCnt = 1;
        m_MemberInfo.append(member);
    }
    RefreshAudioListTabel();
}

void AudioEditor::on_InsertAudio_clicked()
{
    quint8 curDex = ui->AudioListTabel->currentRow();
    if(curDex>=0 && curDex < m_MemberInfo.count())
    {
        Tree_Member_Info member = m_MemberInfo.at(curDex);
        member.mAudioInfo.mSpectrumCnt += 1;
        m_MemberInfo.replace(curDex, member);
        RefreshSpectrumTabel(curDex);
    }
}


void AudioEditor::on_InsertRecord_clicked(bool checked)
{
    if(checked)
    {
        ui->RecordSetGroupbox->setHidden(false);
    }
    else
    {
        ui->RecordSetGroupbox->setHidden(true);
    }
}
