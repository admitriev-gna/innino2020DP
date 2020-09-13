#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

using namespace cv;
using namespace std;

Mat src_gray;
int thresh = 100;
RNG rng(12345);

Point2f obj;
const int gl=9;
int histog[481];

Mat really(Mat tp,Mat gtp){
	//imshow ("tp1",tp);
	//imshow("ori",gtp);
	
	for (int j=0; j<tp.cols;j++)
		for (int i=gl; i<tp.rows-gl;i++)
		{
			if (tp.at<uchar>(i,j)!=0){
				int bel=0;
				while (tp.at<uchar>(i,j)!=0) {if (i+1<tp.rows-gl) {bel++;   i++;} else break;  }
				int sum1=0, sum2=0;
				if (bel>15) for (int kl=1; kl<bel;kl++) tp.at<uchar>(i-kl,j)=0; else
				if ((i+gl<tp.rows)&&(i-bel-gl>=0)) 
				{
				for (int ii=0;ii<gl;ii++)
					sum1=sum1+gtp.at<uchar>(i+ii,j);
				for (int ij=0;ij<gl;ij++)
					sum2=sum2+gtp.at<uchar>(i-ij-bel,j);
				int l=abs(sum1-sum2);
				if (l>(sum1+sum2)*0.1)  for (int kl=0; kl<bel;kl++) tp.at<uchar>(i-kl,j)=0;}
			
			
			
			}


		}
		Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
		//cv::dilate(tp,tp,erodeElement,Point(-1,-1),9);
		cv::erode(tp,tp,erodeElement,Point(-1,-1),1);
		//imshow ("tp",tp);
		//waitKey(5000);
		return tp;

}


int rereally(Mat img)
{
	Mat sob=Mat::zeros(cv::Size(img.cols,img.rows+2*gl),CV_8UC1);
	Rect rec(Rect(0,gl,img.cols,img.rows));
	Mat img2=Mat::zeros(cv::Size(img.cols,img.rows+2*gl),CV_8UC1);
	Mat lap=Mat::zeros(cv::Size(img.cols,img.rows),c);
	Mat sum=Mat::zeros(cv::Size(img.cols,img.rows),CV_8UC1);
	Mat sqsum=Mat::zeros(cv::Size(img.cols,img.rows),CV_8UC1);
	Mat titled=Mat::zeros(cv::Size(img.cols,img.rows),CV_8UC1);
	if (img.channels()!=1) cv::cvtColor(img,img,COLOR_BGR2GRAY);
	Mat tmp1=sob(rec);
	img.copyTo(img2(rec));
	cv::Sobel(img,tmp1,img.depth(),0,1,3);
	cv::threshold(tmp1,sob,50,255,THRESH_BINARY);
	int sumy=0;
	
	sob=really(sob,img2);
	for (int i=0;i<sob.rows;i++)
		for (int j=0;j<sob.cols-1;j++)
		{
			if (sob.at<uchar>(i,j)>0) sumy++;
		}
	if (sumy<sob.cols/3) return 0;
	//cv::Laplacian(img,lap,img.depth());
	cv::integral(sob,sum,sqsum,titled);
	imshow("sasa",sob);
	imshow("sasa2",img);
	imshow("sasa3",lap);
	imshow("sasa3",sum);
	imshow("sasa4",sqsum);
	imshow("sasa5",titled);
	
	
	cv::convertScaleAbs(sqsum,sqsum);
	if (sqsum.channels()!=1) cv::cvtColor(sqsum,sqsum,COLOR_BGR2GRAY);
	int sumx[640];
	for (int i = 0;i<640;i++) sumx[i]=0;
	for (int i=0;i<sqsum.rows;i++)
		for (int j=0;j<sqsum.cols-1;j++)
		{
			if (sqsum.at<uchar>(i,j)>0) sumx[j]++;
		}
	int len=0;

	for (int i = 0;i<640;i++) 
		if ((sumx[i]>0)&&(sumx[i]<15)) {len++; if (len>150) return 2;} else len=0;
	waitKey(1);
	return 1;
}





Mat LineDetect(Mat img, Mat img2)
{
	Mat hist=Mat::zeros(cv::Size(img.cols+2,img.rows+2),CV_8UC3);
	obj.x=0;obj.y=0;
	histog[0]=0;
	histog[1]=0;
	histog[2]=0;
	Mat trololo=Mat::zeros(cv::Size(img.cols,img.rows),CV_8UC3);;
	int sum[490];
	sum[0]=0;
	int summ=0;
	int jk=1;
	for ( int i=0; i<img.rows; i++) 
		for (int j=0; j<img.cols;j++) {
			if (j==0) histog[i+3]=0;
			if (img.at<uchar>(i,j)!=0) {histog[i+1]++;histog[i+2]++;histog[i+3]++;summ++;}
	}
				for (int i=0; i<img.rows; i++)
		{
			if (histog[i]==0) {jk++;sum[jk]=0;} else sum[jk]=sum[jk]+histog[i];
			//if (sum[jk]!=0)cv::line(hist,Point(sum[jk-1]/20,i),Point(sum[jk]/20,i+1),Scalar(0,0,255),1,8,0);
			cv::line(hist,Point(histog[i]/3,i),Point(histog[i+1]/3,i+1),Scalar(255,255,255),1,8,0);
			int olo=0;
			for (int ii=0;ii<jk;ii++) if (sum[ii]!=0) olo++;
			
			summ=std::max(summ*0.1,0.10*img.cols);
			if (histog[i]/3>summ) {
				Mat tmp= Mat::zeros(3+15+15,img.cols,CV_8UC1);
				int b=0;
			while (i-15+b<0) b++;
			while (i-15+b+tmp.rows>=img.rows) b--; 
			
			img2(cv::Rect(0,i-15+b,img.cols,tmp.rows)).copyTo(tmp);
				int prop=rereally (tmp);
				if (prop==2) cv::line(trololo,Point(0,i),Point(trololo.cols,i),Scalar(0,0,255),15,8,0); 
				if (prop==0) cv::line(trololo,Point(0,i),Point(trololo.cols,i),Scalar(255,0,0),3,8,0);
				if (prop==1) cv::line(trololo,Point(0,i),Point(trololo.cols,i),Scalar(0,255,0),5,8,0);}
		}
				//cv::line(hist,Point(summ,0),Point(summ,hist.rows),Scalar(0,255,255),1,8,0);
			

		
		//imshow("hist2",img);
		imshow("hist",hist);
		//waitKey(1);
		return trololo;
}



int main()
{
	cv::VideoCapture cap(0);
	Mat frame,frame1,res;
	
	bool nm=false;
	int t=0;
	int iter=0;
	Mat m_openingKernel(3,7,CV_8UC1);
	Mat m_openingKernel2(3,3,CV_8UC1);
	
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(5,5));
	for (int i=0;i<50;i++)
		cap>>frame1;
	Mat splines;
	Mat bframe2=Mat::zeros(cv::Size(frame1.cols,2*gl+frame1.rows),CV_8UC1);
	Mat res2=Mat::zeros(cv::Size(frame1.cols,2*gl+frame1.rows),CV_8UC1);

	Mat bframe=Mat::zeros(cv::Size(frame1.cols,frame1.rows),CV_8UC1);
	Mat maskk;
	Rect gre(cv::Rect(0,gl,frame1.cols,frame1.rows));
	Rect temp,temp2;
	bframe=bframe2(gre);
	res=res2(gre);
	Mat tmp(frame1.size(),CV_8UC3);
	Mat tmp3(frame1.size(),CV_8UC3);
	Mat tmp4(frame1.size(),CV_8UC3);
	Mat can(frame1.size(),CV_8UC3);
	Mat tmp2(frame1.size(),CV_8UC3);
	Mat ures(frame1.size(),CV_32FC1);
	Mat mask(frame1.size(),CV_32FC1);
	cv::cvtColor(frame1,frame1,COLOR_BGR2GRAY);
	for (;;)
	{
		cap>>frame;
		cv::Point p1,p2;
		maskk=Mat::zeros(cv::Size(frame1.cols,frame1.rows),CV_8UC1);
//		if (nm==true){iter=MovDetect(frame,frame1,iter);
//		if (iter==5) {
//			cout<<"  dvig na  "<<obj.x<<"   "<<obj.y<<endl;
//			circle(frame,Point(obj.x,obj.y),20,Scalar(0,255,0),2);
//		}}
		std::vector<cv::Rect> targets;
		//frame.copyTo(bframe);
		//cv::GaussianBlur(frame,bframe,cv::Size(3,3),-1);
		cv::cvtColor(frame,bframe,COLOR_BGR2GRAY);
		
		cv::absdiff(bframe,frame1,res);
		//cv::cvtColor(res,res,COLOR_BGR2GRAY);
		cv::threshold(res,res,5,255,THRESH_BINARY);
		//cv::erode(res,res,erodeElement,Point(-1,-1),1);
		//cv::dilate(res,res,erodeElement,Point(-1,-1),3);
		cv::morphologyEx(res,res,cv::MORPH_CLOSE, cv::Mat());
		cv::morphologyEx(res,res,cv::MORPH_OPEN,m_openingKernel2,cv::Point(-1,-1),1,cv::BORDER_CONSTANT,cv::Scalar(0));
		t++;Scalar color = Scalar(100,50,200); Scalar color2 = Scalar(100,200,50);
		//imshow("lololo", res);
		cv::updateMotionHistory(res,ures,t,3.); 

		cv::segmentMotion(ures,mask,targets,t,1.);
		if (!targets.empty())  {
			//cv::cvtColor(mask,mask,COLOR_GRAY2BGR);
			int tr; if (targets.size()<10) tr=targets.size(); else tr=10;//=std::min(10,targets.size());
			int maxx=0, minx=3000,maxy=0,miny=639*3;
			for(int j =  0; j<tr;j++) {
				//if (targets[j].width<20) ures(cv::Rect(targets[j]))=;
				if ((targets[j].width>frame.cols/4)){
				if (targets[j].x<minx) minx=targets[j].x;
				if (targets[j].y<miny) miny=targets[j].y;
				if ((targets[j].x+targets[j].width)>maxx) maxx=targets[j].x+targets[j].width;
				if ((targets[j].y+targets[j].height)>maxy) maxy=targets[j].y+targets[j].height;
				//Rect temp = Rect(targets[j].x,targets[j].y,targets[j].width,targets[j].height);
				Mat lolol=maskk(Rect(targets[j].x,targets[j].y,targets[j].width,targets[j].height));
				temp = Rect(cv::Rect(targets[j].x,targets[j].y,targets[j].width,targets[j].height+gl*2));
				res2(temp).copyTo(tmp);
				//tmp=res2(temp);
				//cv::threshold(tmp,tmp,20,255,THRESH_BINARY);
				//tmp=ures(cv::Rect(targets[j].x,targets[j].y,targets[j].width,targets[j].height));
				//int k = tmp.channels();
				
				tmp2=bframe2(temp);
				cv::Canny(tmp2,can,2,255);

				if (can.channels()==3) 
					cv::cvtColor(can,can,COLOR_BGR2GRAY);
				if (tmp2.channels()==3) 
					cv::cvtColor(tmp2,tmp2,COLOR_BGR2GRAY);
				if (tmp.channels()==3) 
					cv::cvtColor(tmp,tmp,COLOR_BGR2GRAY);
				//cv::dilate(can,can,erodeElement,Point(-1,-1),1);
				cv::morphologyEx(can,can,cv::MORPH_CLOSE, cv::Mat());
				cv::morphologyEx(can,can,cv::MORPH_OPEN,m_openingKernel,cv::Point(-1,-1),1,cv::BORDER_CONSTANT,cv::Scalar(0));
				cv::addWeighted(can,0.5,tmp,0.5,1,tmp);
				cv::threshold(tmp,tmp,250,255,THRESH_BINARY);
				cv::dilate(tmp,tmp,erodeElement,Point(-1,-1),1);
				//imshow("tmp", tmp);
				//cvtColor(tmp,tmp,COLOR_BGR2GRAY,1);
				//int k= tmp.at<uchar>(3,3);
			//	cout<<k;
				tmp=really(tmp,tmp2);
				//cv::cvtColor(tmp2,tmp2,COLOR_GRAY2BGR);
				//cv::split(tmp2,splines);
				temp2 = Rect(cv::Rect(0,gl,targets[j].width,targets[j].height));
				tmp3=tmp(temp2);
				tmp4=tmp2(temp2);
				tmp3.copyTo(lolol);
				//maskk(Rect(targets[j].x,targets[j].y,targets[j].width,targets[j].height))=tmp3;
				//cv::addWeighted(tmp3,1,tmp4,1,1,tmp4);
				//cv::merge(splines,3,tmp2);
				//imshow("kilki", tmp3);
				//LineDetect(tmp);
				//if ((obj.x!=0)&&(obj.y!=0))
				//cv::line(frame,Point(targets[j].x,obj.x+targets[j].y),Point(targets[j].x+targets[j].width,obj.y+targets[j].y),cv::Scalar(255,0,0),8,30);
				}
				//cv::rectangle(frame,targets[j],color2,2,8,0);
				}
			p1=Point(minx,miny);
			p2=Point(maxx,maxy);
		}
		
		splines=LineDetect(maskk,frame);
		cv::addWeighted(maskk,1,bframe,1,1,maskk);
		cv::addWeighted(splines,1,frame,1,1,splines);
		imshow("poip3",maskk);
		cv::rectangle(splines,p1,p2,color,2,15,0);
		imshow("poip",splines);
		//imshow("poip2",res);
		bframe.copyTo(frame1);
		int l=waitKey(10);
		if (l==27) break; else  if (l>27) nm=!nm;

	}
	return 1;
}