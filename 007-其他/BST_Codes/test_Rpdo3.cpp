UNS8 realtimeMsg[2][24];
UNS8 fullscrennMsg[4][16];
int mCurline;
int mCurcolumn;
UNS8 mLastRealChars[6];
UNS8 mLastFullChars[6];
// rpdo3 rpdo4 会不会同时到来
bool judgeRpdoIsChange(int curline, int curcolumn, int isReal)
{
    bool flag = false;
    if (1 == isReal)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (mLastRealChars[i] != realtimeMsg[curline][curcolumn + i])
            {
                for (int j = i; j < 6; ++j) // 如果改变了，从改变的那一位开始保存数值
                {
                    mLastRealChars[i] != realtimeMsg[curline][curcolumn + j];
                }
                flag = false;
                break;
            }
            else
            {
               flag = true;
            }
        }
        if (mCurline != curline || mCurcolumn != curcolumn || !flag) // 如果任意一个值改变了
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (0 == isReal)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (mLastFullChars[i] != fullscrennMsg[curline][curcolumn + i])
            {
                for (int j = i; j < 6; ++j)// 如果改变了，从改变的那一位开始保存数值
                {
                    mLastFullChars[i] != fullscrennMsg[curline][curcolumn + j];
                }
                flag = false;
                break;
            }
            else
            {
               flag = true;
            }
        }
        if (mCurline != curline || mCurcolumn != curcolumn || !flag) // 如果任意一个值改变了
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void OpenClient::buildMessageAndEmitSignal(bool pflag)
{
    QString ba1;
    QString ba2;

    if(0 == m_Display_Mode_FCT_AFF)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 24; ++j)
            {
                if (0 == i)
                {
                    ba1.append(realtimeMsg[i][j]);
                }
                else if (1 == i)
                {
                    ba2.append(realtimeMsg[i][j]);
                }
            }
        }
        if(pflag)
        {
            emit sigRealMessage(ba1, ba2);
        }
    }
    else
    {
        QString ba3;
        QString ba4;
        for (int i = 0; i < 4; ++i)
        {
            for(int j=0; j<16; j++)
            {
                if (0 == i)
                {
                    ba1.append(fullscrennMsg[i][j]);
                }
                else if (1 == i)
                {
                    ba2.append(fullscrennMsg[i][j]);
                }
                else if (2 == i)
                {
                    ba3.append(fullscrennMsg[i][j]);
                }
                else if (3 == i)
                {
                    ba4.append(fullscrennMsg[i][j]);
                }
            }
        }
      if(pflag)
        {
            emit sigMessage(ba1, ba2, ba3, ba4);
        }
    }
}

void OpenClient::myRPDO3Arrived()
{
    UNS8 curLine = text_segmented_1_MSG1_LIGNE;     // 哪一行
    UNS8 curClume = text_segmented_1_MSG1_COLONNE;  // 哪一列
    if(0 == m_Display_Mode_FCT_AFF)    // 小屏文本
    {
        if (curLine > 2 || curClume > 23) // 如果小屏文本超过2行或者列数超过24列，不做处理
        {
            return;
        }
        else
        {
            realtimeMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            realtimeMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            realtimeMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            realtimeMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
            realtimeMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
            realtimeMsg[curLine][curClume+5] = text_segmented_1_MSG1_CHAR6;
            if (judgeRpdoIsChange(curline, curcolumn, 1)) // 如果小屏文本改变了
            {
                // 改变了去通知刷新最新的文本
                buildMessageAndEmitSignal(true);
            }
        }
    }
    else
    {
        if (curLine > 3 || curClume > 15)  // 如果大屏文本超过4行或者列数超过16列，不做处理
        {
            return;
        }
        else
        {
            fullscrennMsg[curLine][curClume] = text_segmented_1_MSG1_CHAR1;
            fullscrennMsg[curLine][curClume+1] = text_segmented_1_MSG1_CHAR2;
            fullscrennMsg[curLine][curClume+2] = text_segmented_1_MSG1_CHAR3;
            fullscrennMsg[curLine][curClume+3] = text_segmented_1_MSG1_CHAR4;
            fullscrennMsg[curLine][curClume+4] = text_segmented_1_MSG1_CHAR5;
            fullscrennMsg[curLine][curClume+5] = text_segmented_1_MSG1_CHAR6;
            if (judgeRpdoIsChange(curline, curcolumn, 0))  // 如果大屏文本改变了
            {
                // 改变了去通知刷新最新的文本
                buildMessageAndEmitSignal(true);
            }
        }
    }
}


void OpenClient::myRPDO4Arrived()
{
    UNS8 curLine = texte_segmented_2_MSG2_LIGNE;     // 哪一行
    UNS8 curClume = texte_segmented_2_MSG2_COLONNE;  // 哪一列
    if(0 == m_Display_Mode_FCT_AFF)    // 小屏文本
    {
        if (curLine > 2 || curClume > 23) // 如果小屏文本超过2行或者列数超过24列，不做处理
        {
            return;
        }
        else
        {
            realtimeMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            realtimeMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            realtimeMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            realtimeMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
            realtimeMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
            realtimeMsg[curLine][curClume+5] = texte_segmented_2_MSG2_CHAR6;
            if (judgeRpdoIsChange(curline, curcolumn, 1)) // 如果小屏文本改变了
            {
                // 改变了去通知刷新最新的文本
                buildMessageAndEmitSignal(true);
            }
        }
    }
    else
    {
        if (curLine > 3 || curClume > 15)  // 如果大屏文本超过4行或者列数超过16列，不做处理
        {
            return;
        }
        else
        {
            fullscrennMsg[curLine][curClume] = texte_segmented_2_MSG2_CHAR1;
            fullscrennMsg[curLine][curClume+1] = texte_segmented_2_MSG2_CHAR2;
            fullscrennMsg[curLine][curClume+2] = texte_segmented_2_MSG2_CHAR3;
            fullscrennMsg[curLine][curClume+3] = texte_segmented_2_MSG2_CHAR4;
            fullscrennMsg[curLine][curClume+4] = texte_segmented_2_MSG2_CHAR5;
            fullscrennMsg[curLine][curClume+5] = texte_segmented_2_MSG2_CHAR6;
            if (judgeRpdoIsChange(curline, curcolumn, 0))  // 如果大屏文本改变了
            {
                // 改变了去通知刷新最新的文本
                buildMessageAndEmitSignal(true);
            }
        }
    }
}
