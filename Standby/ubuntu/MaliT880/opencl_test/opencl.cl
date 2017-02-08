

__kernel void hist_local(__global uchar16 *img_ptr0,__global uchar16 *img_ptr1,__global uchar16 *img_ptr2,int w,int h) 

{

	int i,id;// 1-D
	uchar16 data0,data1,data2;

id=get_global_id(0)*4;

for(i=0;i<4;i++,id++)
{
data0=img_ptr0[id];
data1=img_ptr1[id];
data2=add_sat(data0,data1);
img_ptr2[id]=data2;
}


}


