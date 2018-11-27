#include "widget.h"
#include "ui_widget.h"
//#include "HeaderFile/gmp.h"
//#include "HeaderFile/pbc.h"
#include <pbc/pbc.h>
#include <gmp.h>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mRunPath = QCoreApplication::applicationDirPath();
    if (!mRunPath.endsWith("/"))
    {
        mRunPath.append("/");
    }
}

Widget::~Widget()
{
    delete ui;
}

int Widget::TestGmp()
{
    mpz_t n;
    char argv[20] = "123456789";
    /* 初始化10进制大整数n，并且把命令行的第一个参数赋值给它*/
    mpz_init(n);
    if (mpz_set_str(n, argv, 10) != 0)
        return -1;

    /* 打印大整数n的值 */
    printf ("n = ");
    mpz_out_str(stdout, 10, n);
    printf ("\n");

    /* 计算(n + 1)的平方 */
    mpz_add_ui(n, n, 1);
    mpz_mul(n, n, n);

    /* 打印(n + 1)平方的值 */
    printf ("(n + 1) ^ 2 = ");
    mpz_out_str(stdout, 10, n);
    printf("\n");

    char buffer[128] = {0};
    mpz_get_str(buffer, 2, n);
    qDebug("As String: %s\n", buffer);
    qDebug("len = %ld", strlen(buffer));
    /* 清除和释放大整数n */
    mpz_clear(n);

    return 0;

}

int Widget::TestPbc()
{

    mpz_t z,V,M;
    element_t s,r,P,Ppub,Qid,dID,gID,gIDr,U,tem1;
    pairing_pp_t pp;

    pairing_t pairing;
    char param[16384];
    qDebug()<<"mRunPath = "<<mRunPath;
    QString tmp = mRunPath + "param/a.param";
    qDebug()<<"tmp = "<<tmp;
    FILE *fp = fopen(tmp.toLatin1().data(), "r");
    if (!fp)
    {
        qDebug("open file error lines = %d", __LINE__);
    }
    size_t count = fread(param, 1, 16384, fp);
    qDebug("count is %ld", count);
    if (!count)
    {
        pbc_die("input error");
    }
    pairing_init_set_buf(pairing, param, count);


    //整数
    mpz_init(z);
    mpz_init(V);
    //初始化并给消息赋值
    mpz_init_set_str(M, "201806014", 0);
    gmp_printf ("M: %Zd\n", M);

    //在1和r之间的整数
    element_init_Zr(s, pairing);
    element_init_Zr(r, pairing);

    //循环群G1
    element_init_G1(P, pairing);
    element_init_G1(Ppub, pairing);
    element_init_G1(Qid, pairing);
    element_init_G1(dID, pairing);
    element_init_G1(U, pairing);

    //循环群GT
    element_init_GT(gID, pairing);
    element_init_GT(gIDr, pairing);
    element_init_GT(tem1, pairing);

    //Setup, system parameters generation
    printf("SETUP STAGE\n");
    element_random(P);
    element_random(s);
    element_mul_zn(Ppub, P, s);//Ppub=sP

    //Extract, key calculation
    printf("EXTRACT STAGE\n");
    char a[2] = {"A"};
    element_from_hash(Qid, &a, 1);//Qid=H("A")
    element_mul_zn(dID, Qid, s);

    //Encrypt encrypt M with ID
    printf("Encrypt STAGE\n");
    element_random(r);
    element_mul_zn(U, P, r);
    pairing_pp_init(pp, Ppub, pairing);
    pairing_pp_apply(gID, Qid, pp); //gID=e(Qid,Ppub)
    pairing_pp_clear(pp);
    element_mul_zn(gIDr, gID, r); //gIDr=gID^r
    element_to_mpz(z,gIDr); //H(gIDr)
    mpz_xor(V,M,z); //V=M xor z
    element_printf("U: %B\n", gIDr);
    gmp_printf ("V: %Zd\n", V);

    //Decrypt decrypt C = <U,V>
    printf("Decrypt STAGE\n");
    pairing_pp_init(pp, U, pairing);
    pairing_pp_apply(tem1,dID, pp);
    pairing_pp_clear(pp);
    element_to_mpz(z,tem1);
    mpz_xor(M,V,z);
    gmp_printf("M: %Zd\n", M);

    //释放内存
    mpz_clear(z);
    mpz_clear(V);
    mpz_clear(M);
    element_clear(s);
    element_clear(r);
    element_clear(P);
    element_clear(Ppub);
    element_clear(Qid);
    element_clear(dID);
    element_clear(gID);
    element_clear(gIDr);
    element_clear(U);
    element_clear(tem1);

    return 0;
}

int Widget::generateBigPrime()
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt); //设置随机数生成算法为默认
    gmp_randseed_ui(grt, time(NULL)); //设置随机化种子为当前时间

    mpz_t key_p, key_q;
    mpz_init(key_p); //初始化q和p大素数
    mpz_init(key_q);

    mpz_urandomb(key_p, grt, 1024);//随机生成一个0-2^1024的一个数
    mpz_urandomb(key_q, grt, 1024);



    //mpz_t key_pp, key_qq;
    mpz_nextprime(key_p, key_p);  //使用GMP自带的素数生成函数
    mpz_nextprime(key_q, key_q);
    gmp_printf("%ZX\n\n", key_p);
    gmp_printf("%ZX\n\n", key_q);

    //计算p*q的值，并存放在n中
    mpz_t key_n;
    mpz_init(key_n);
    mpz_mul(key_n, key_p, key_q); //计算p*q

    //计算(p-1)*(q-1)并将值放在key_f 中
    mpz_t key_f;
    mpz_init(key_f);
    mpz_sub_ui(key_p, key_p, 1);
    mpz_sub_ui(key_q, key_q, 1);
    mpz_mul(key_f, key_p, key_q);

    //找出符合要求的e，即公钥为(e,n) e通常取3,17和65537三个值，我们直接取e=65537
    mpz_t key_e;
    mpz_init_set_ui(key_e, 65537);

    //输出公钥（e,n)
    gmp_printf("%s (%ZX, %ZX)\n\n\n", "public key is:", key_n, key_e);

    //求e的逆元，即ed mod (f)=1;
    //用gmp自带的求数论逆元函数对其进行求解
    mpz_t key_d;
    mpz_init(key_d);
    mpz_invert(key_d, key_e, key_f);
    gmp_printf("%s (%ZX, %ZX)\n\n\n", "private key is:", key_d, key_e); //输出私钥


    //将明文m进行加密 C=m^e mod n
    mpz_t M, C;
    mpz_init(C);
    mpz_init_set_ui(M, 1234);
    //mpz_get_str(M, 16, M);
    mpz_powm(C, M, key_e, key_n);  //使用GMP中的模幂计算函数 C=M^e mod n;
    gmp_printf("%s %ZX\n\n", "the cipertxt is", C);

    //解密函数算法 M=C^d mod n
    mpz_t M2;
    //mpz_init_set_str(C2,M2,16);
    mpz_init(M2);
    mpz_powm(M2, C, key_d, key_n);   //使用GMP中的模幂计算函数 M=C^d mod n;
    gmp_printf("%s %ZX\n\n","the M2 is", M2);

    mpz_clear(key_q);
    mpz_clear(key_p);
    mpz_clear(M);
    mpz_clear(C);
    mpz_clear(M2);
    mpz_clear(key_n);
    mpz_clear(key_f);
    mpz_clear(key_d);
    return 0;
}

