/*
gcc -o wave wave.c
./wave ImpulseResponse.raw
*/

/*
注意

出力ファイル名の指定もすること

音ファイルのbit数に合わせて、short型、double型を変更するのを忘れずに
70行付近のfprintf内の型にも注意
*/

#include<stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define bit 16 //ビット数
#define sample_L 0.0208333 //標本間隔(tミリ秒)

int main(int argc, char *argv[]){
  /* argv[0]はプログラム名 */
  /* argv[1]は第一引数名で入力するファイル名 */

  FILE *fp;
  FILE *fp2;
  int i;
  int k;
  //バイト数分、動的メモリ確保する変数
  struct stat filesize;
  //バイトの計算用変数
  int byte = 0;
  //rawファイルを分割し、一つ5秒で表現
  //short *wave;
  short wave[480000]={};
  //出力するファイル名
  short out[240000]={};
  //何個のrawファイルを作成するか
  int size=0;
  //どこからよみこむか
  int set=0;
  //ファイル名
  char name[1000];
  
  //バイトの計算
  byte = bit / 8;
  
  //ファイルの状態をstatによって構造体に保存
  //http://www.c-lang.net/stat/index.html
  stat(argv[1], &filesize);
  //2バイトで1標本なので「.st_size」を使い標本数を求める
  //しかし、ステレオなので、本当のファイルサイズは1/2倍
  //5秒ごとに区切るのでサイズは240000で割る。
  size = ( (filesize.st_size / byte)*0.5 ) / 240000;
  
  //ファイルオープン
  if((fp = fopen( argv[1], "rb" ))==NULL)
   {
      printf("ファイルを開くことができません\n");
      exit(1);
   }
   
   //waveにオープンする
   //wave = ( short *)malloc( sizeof(short) * (filesize.st_size / byte) );
   //fread( wave, 2, (filesize.st_size / byte), fp);
  
  for(i=0; i<size; i++)
  {
     sprintf(name, "%03d_nashi_front_15min_16bit.raw", i);
     
      //ファイルオープン
     if((fp2 = fopen( name, "w" ))==NULL)
     {
      printf("ファイルを開くことができません\n");
      exit(1);
     }
  
    //読み込み位置を計算
    set = 480000*i;
    
     //fseekより指定された位置から読み込むようにセット
     fseek(fp, set, SEEK_SET);
     //480000=240000のステレオのデータ
     fread( wave, 2, 480000, fp );
     
     /*
     for(k=0; k<480000; k++)
     {
       printf("k=%d, wave=%d\n", k, wave[k]);
     }
     */
     
     for(k=0; k<240000; k++)
     {
     	out[k] = (short) (wave[k*2] + wave[k*2+1]) / 2;
     	
     }
     
     fwrite( out, 2, 240000, fp2);
     
     for(k=0; k<240000; k++)
     {
     	out[k] = 0;
     }
     
     for(k=0; k<480000; k++)
     {
     	wave[k] = 0;
     }
     
     fclose(fp2);

  }
  
  
  fclose(fp);  


  return 0;
}
