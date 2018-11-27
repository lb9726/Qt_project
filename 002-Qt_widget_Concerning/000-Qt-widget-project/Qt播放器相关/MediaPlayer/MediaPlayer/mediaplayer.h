#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>


namespace Ui {
class MediaPlayer;
}

class MediaPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();

private slots:
    void on_pushButton_open_file_clicked();

    void on_pushButton_play_and_pause_clicked();

    void onTimerOut();

    void slider_progress_clicked();

    void slider_progress_moved();

    void slider_progress_released();

    void on_pushButton_volume_clicked();

    void slider_volume_changed();

private:
    Ui::MediaPlayer *ui;
};

#endif // MEDIAPLAYER_H
