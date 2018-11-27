#ifndef D_GraphicsComponent_H
#define D_GraphicsComponent_H

#include "uifun.h"
#ifdef SUPPORT_SCENE
class GraphicsScene;
#elif SUPPORT_COMEDIT
class DiagramScene;
#endif

class GraphicsComponent : public QGraphicsWidget, public RcContainer
{
    Q_OBJECT
public:
#ifdef  SURRPORT_3D
    friend class GraphicsGLWidget;
#endif
    GraphicsComponent(QGraphicsItem *parent, GraphicsScene *pScene, QRectF pRect);
    ~GraphicsComponent();

    void ReleaseTextures();

    QRectF boundingRect() const;
    QPainterPath shape() const;

    bool ModifyGeomotery(QStringList pGeoList);
    virtual void ModifyGeomotery(QRectF pRect);
    virtual void SetComGeometory(QRectF pRect);

#ifdef  SURRPORT_3D
    void setGeometry3D(qreal x, qreal y, qreal width, qreal height);
    void setGeometry3D(QRectF rect);
    void BindTexture(GLuint L_Texture,GLfloat R,GLfloat G,GLfloat B);
    void UnbindTexture();
#endif

    void Restart();
    virtual bool Start();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual bool PaintEffect(QPainter *p);

#ifdef  SURRPORT_3D
    void OpenGL_PaintBegin(QPainter *painter);
    void OpenGL_PaintEnd(QPainter *painter);
    void OpenGL_Effect(QPainter *painter);
#endif

    void Play();
    void Stop();
    bool SetEnvironment(QString pEnvirString);  //>@设置COM的现场环境
    bool GetEnvironment(QString &pEnvirString); //>@获取COM的现场环境

    bool SetElementText(QDomElement pElement, QString pValue);

    //>@获取本节点中的重要信息
    virtual QString GetInformation() { return QString("%1").arg(m_ComElement.tagName()); }

    //>@任何动作最多只需要两步，如果有两步，则第一步肯定是状态CHANGE，第二步一定是状态的操作变化
    void OperateStep0();

    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

public:
    void PressEvent(QGraphicsSceneMouseEvent *event);
    void MoveEvent(QGraphicsSceneMouseEvent *event);
    void ReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void sUpdateCom(int pFrame);

    void sSetDevices(bool pEnable, E_DEVMODE pDevMode);

    void sInfo(QString pMsg, int pDelay=3000, E_DEVMODE pMode=DM_NORMAL);
    void sWarn(QString pContent, int pDelay=3000, E_DEVMODE pMode=DM_NORMAL);
    void sError(QString pContent, int pDelay=3000, E_DEVMODE pMode=DM_NORMAL);
    void sProgress(quint32 pValue);

    void sBeep(QString);  //>@发送播报beep信号
    void sIcon(QString);  //>@发送显示图标信号

    void MouseRelease(QPointF Pos);
    void reportEffect(AREA_OPERATE pOperate, QVariant pPara);

    void sRestart();
    void sModifyGeomotery(QRectF pRect);

public:
    virtual void OperateTurn(QVariant pPara);
    virtual void OperateShow(QVariant pPara);
    virtual void OperateHide(QVariant pPara);
    virtual void OperateClear(QVariant pPara);
    virtual void OperatePlay(QVariant pPara);
    virtual void OperateStop(QVariant pPara);

public slots:
    void slot_Initialize() {}
    void slot_Restart();
    void slot_ModifyGeomotery(QRectF pRect);
    virtual void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    virtual void UpdateCom(int pFrame);
    virtual void ExecAction(QAction *pAction) {}
    void OperateStep1();

public:
    QTimeLine                           m_Animator;
    AREA_ANIMATE                       *m_AreaAnimate;
    OPERATE_INFO                        m_OperateInfo[2];

    //>@用于UpdateEffect中
    AREA_OPERATE                        m_UpdateOPerate;
    QVariant                            m_UpdatePara;

    bool                                m_PaintOptimizer; //>@优化绘制操作

    QActionGroup*                       m_ActionGroup;

public:
#ifdef  SURRPORT_3D
    GraphicsGLWidget                   *m_GLWidget;
    int                                 m_3DWidth, m_3DHeight;
    //>@用于XYZ三轴旋转以及深度的变化
    int                                 m_3DXDiff,m_3DYDiff,m_3DZDiff; //>@三个方向上的角度偏移
    qreal                               m_3DXScale,m_3DYScale,m_3DZScale; //>@三个方向上的缩放
    float                               m_3DDeepDiff;
    GLfloat                             m_3DVerPoint[7200];
    GLfloat                             m_3DVerTexture[7200];
    GLfloat                             m_3DNormalFront[7200];
#endif

public:
    void StartTimeline();
    void StopAnimate();

    QPixmap GetFlipPixmap(QPixmap pPixmap, QRectF pTargetRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QSizeF pSize, QRectF pTargetRect, QRectF pSourceRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect, QRectF pSourceRect);
    bool DrawFlipPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRectF pTargetRect);
    bool DrawPixmap(QPainter *p, QPixmap pPixmap, QRect pTargetRect) {  return DrawPixmap(p, pPixmap, QRectF(pTargetRect)); }
    bool DrawPixmap(QPainter *p, QPixmap pPixmap);

    void Paint2D_None(QPainter *p, QRectF targetRect);
    void Paint2D_Flip(QPainter *p,QRectF targetRect);
    void Paint2D_TileRoll(QPainter *p, QRectF targetRect);
    void Paint2D_Roll(QPainter *p,QRectF targetRect);
    void Paint2D_Zoom(QPainter *p,QRectF targetRect);
    void Paint2D_Rotate(QPainter *p,QRectF targetRect);
    void Paint2D_Twirl(QPainter *p, QRectF targetRect);
    void Paint2D_Blink(QPainter *p,QRectF targetRect);
    void Paint2D_Slipcycle(QPainter *p,QRectF targetRect);
    void Paint2D_Fireworks(QPainter *p, QRectF targetRect);
    void Paint2D_Transparent(QPainter *p,QRectF targetRect);
    void Paint2D_Spread(QPainter *p,QRectF targetRect);
    void Paint2D_Wheel(QPainter *p,QRectF targetRect);

#ifdef  SURRPORT_3D
    void Paint3D_CircleSurface(GLuint texture);//>@绘制圆面
    void Paint3D_CamberSurface(GLuint texture);//>@绘制弧面
    void Paint3D_Cube(QPainter *p,QRectF targetRect);    //>@绘制立方体
    void Paint3D_Cylinder(QPainter *p,QRectF targetRect);//>@绘制圆柱体
    void Set3DEffect(EffectType pType);
#endif
};

#ifdef  SURRPORT_3D

class GraphicsGLWidget : public QGLWidget
{
public:
    explicit GraphicsGLWidget() : QGLWidget()
    {
    }
    ~GraphicsGLWidget()
    {
    }
    bool InitTexture(GraphicsComponent *pGraphicsWidget)
    {
        if(pGraphicsWidget = 0)
            return false;
        QList<int> tmpKeyList;// = pGraphicsWidget->m_EffectPath.keys();
        int count = tmpKeyList.count();
        int tmpKey;
        for(int i=0;i<count;i++)
        {
            tmpKey = tmpKeyList.at(i);
            QPixmap tmpPixmap;
            //tmpPixmap.load(pGraphicsWidget->m_EffectPath.value(tmpKey));
            if(tmpPixmap.isNull())
                continue;
            GLuint tmpGLuint = bindTexture(tmpPixmap);
            //pGraphicsWidget->m_EffectTexture.insert(tmpKey, tmpGLuint);
        }
        return true;
    }
    GLuint GetTexture(QString pImagePath, QSizeF pSize)
    {
        QImage tmpImage(pImagePath);
        if(tmpImage.isNull())
        {
            IDE_TRACE_STR(pImagePath);
            return 0;
        }
        return bindTexture(QPixmap::fromImage(tmpImage.scaled(pSize.toSize())));
    }
};

#endif



#ifdef  SURRPORT_3D
inline void GraphicsComponent::Paint3D_CircleSurface(GLuint texture)
{
    glPushMatrix();
    glFrontFace(GL_CCW);
    glTranslatef(-0.5f,-0.5f,0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //本质上是画360个点，来画360个三角形。
    for(int i=0;i<360;i+=2)
    {
        double aa=(double)PI*i/180;
        m_3DVerPoint[i]=(float)((cos(aa)+1.0)*0.5);   //纹理横坐标
        m_3DVerPoint[i+1]=(float)((sin(aa)+1.0)*0.5);  //纹理纵坐标
    }
    glVertexPointer(2, GL_FLOAT, 0, m_3DVerPoint);
    BindTexture(texture,1.0f,1.0f,1.0f);
    glTexCoordPointer(2,GL_FLOAT,0,m_3DVerPoint);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 180);
    UnbindTexture();
    glPopMatrix();
}

inline void GraphicsComponent::Paint3D_CamberSurface(GLuint texture)
{
    int fragAngle = 30;
    int n = 180/fragAngle;
    float b = (float)fragAngle/180;
    float r = (float)1/(2*PI)-0.005;
    glPushMatrix();
    glRotatef(-90,0.0f,1.0f,0.0f);
    glScalef(2*PI, 1.0f, 2*PI);
    glTranslatef(0.0f,0.5f,0.0f);
    for(int j=0;j<2;j++)
    {
        for(int i=0;i<n;i++)
        {
            m_3DVerPoint[0] = -0.0;m_3DVerPoint[1] = 0.0;m_3DVerPoint[2] = r;
            m_3DVerPoint[3] = b;m_3DVerPoint[4] = -1.0;m_3DVerPoint[5] = r;
            m_3DVerPoint[6] = -0.0;m_3DVerPoint[7] = -1.0;m_3DVerPoint[8] = r;

            m_3DVerPoint[9] = b;m_3DVerPoint[10] = -1.0;m_3DVerPoint[11] = r;
            m_3DVerPoint[12] = -0.0;m_3DVerPoint[13] = 0.0;m_3DVerPoint[14] = r;
            m_3DVerPoint[15] = b;m_3DVerPoint[16] = 0.0;m_3DVerPoint[17] = r;
            GLfloat NormalFront[] = {
                                        0,0,-1, 0,0,-1, 0,0,-1,
                                        0,0,-1, 0,0,-1, 0,0,-1
                                    };
            memcpy(m_3DVerTexture,m_3DVerPoint,18*sizeof(GLfloat));
            m_3DVerTexture[0] = m_3DVerPoint[0]+b*i;
            m_3DVerTexture[3] = m_3DVerPoint[3]+b*i;
            m_3DVerTexture[6] = m_3DVerPoint[6]+b*i;

            m_3DVerTexture[9] = m_3DVerPoint[9]+b*i;
            m_3DVerTexture[12] = m_3DVerPoint[12]+b*i;
            m_3DVerTexture[15] = m_3DVerPoint[15]+b*i;

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3,GL_FLOAT,0,m_3DVerPoint);
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT,0,NormalFront);
            BindTexture(texture,1.0f,1.0f,1.0f);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(3,GL_FLOAT,0,m_3DVerTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glRotatef(fragAngle, 0, 1, 0);
        }
    }
    UnbindTexture();
    glPopMatrix();
}

inline void GraphicsComponent::Paint3D_Cube(QPainter *p,QRectF targetRect)
{
    OpenGL_PaintBegin(p);
    OpenGL_Effect(p);

    glPushMatrix();
    //glRotatef(180,0.0f,0.0f,1.0f);
    glPushMatrix();
    glFrontFace(GL_CCW);
    GLfloat afVerticesFront[] = {
                                    -0.5f,  0.5f,   0.5f,
                                    0.5f,   0.5f,   0.5f,
                                    -0.5f,  -0.5f,  0.5f,
                                    0.5f,   -0.5f,  0.5f,
                                };
    GLfloat afNormalsFront[] = {
                                    0,0,1, 0,0,1, 0,0,1,
                                    0,0,1, 0,0,1, 0,0,1
                                };
    GLfloat afTexCoordFront[] = {
                                    0.0f, 1.0f,
                                    1.0f, 1.0f,
                                    0.0f, 0.0f,
                                    1.0f, 0.0f
                                };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //>@Front
    glVertexPointer(3,GL_FLOAT,0,afVerticesFront);
    glNormalPointer(GL_FLOAT,0,afNormalsFront);
    //BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(0), targetRect.size()),1.0f,1.0f,1.0f); //CubeFront
    glTexCoordPointer(2,GL_FLOAT,0,afTexCoordFront);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Top
    glPopMatrix();
    //BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(1), targetRect.size()),1.0f,1.0f,1.0f);//CubeTop
    glRotatef(90,1.0f,0.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Back
    glPopMatrix();
    //BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(2), targetRect.size()),1.0f,1.0f,1.0f);//CubeBack
    glRotatef(180,1.0f,0.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Bottom
    glPopMatrix();
    //BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(3), targetRect.size()),1.0f,1.0f,1.0f);//CubeBottom
    glRotatef(270,1.0f,0.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Right
    glPopMatrix();
    //BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(4), targetRect.size()),1.0f,1.0f,1.0f);//CubeRight
    glRotatef(90,0.0f,1.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    //>@Left
    glPopMatrix();
    //BindTexture(m_GLWidget->GetTexture(m_EffectPath.value(5), targetRect.size()),1.0f,1.0f,1.0f);//CubeLeft
    glRotatef(270,0.0f,1.0f,0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    UnbindTexture();
    glPopMatrix();
    glPopMatrix();

    OpenGL_PaintEnd(p);
}

inline void GraphicsComponent::Paint3D_Cylinder(QPainter *p,QRectF targetRect)
{
    OpenGL_PaintBegin(p);
    OpenGL_Effect(p);

    glPushMatrix();
    glScalef(0.5, 0.2, 0.5);
    glPushMatrix();
    //Paint3D_CamberSurface(m_GLWidget->GetTexture(m_EffectPath.value(0), targetRect.size()));//SideSurface
    glTranslatef(0.0f,0.5f,0.0f);
    glRotatef(90,1.0f,0.0f,0.0f);
    glRotatef(180,0.0f,0.0f,1.0f);
    glScalef(1.94, 1.94, 1);
    //Paint3D_CircleSurface(m_GLWidget->GetTexture(m_EffectPath.value(1), targetRect.size()));//TopCircleSurface
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f,-0.5f,0.0f);
    glRotatef(-90,1.0f,0.0f,0.0f);
    glScalef(1.94, 1.94, 1);
    //Paint3D_CircleSurface(m_GLWidget->GetTexture(m_EffectPath.value(2), targetRect.size()));//BottomCircleSurface
    glPopMatrix();
    glPopMatrix();

    OpenGL_PaintEnd(p);
}
#endif

#endif // GraphicsComponent_H
