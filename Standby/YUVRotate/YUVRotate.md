## 如何划分工作组


工作组的大小（工作组中包含的工作项数目），


工作组中总的工作项数目为 local_work_size[0] * local_work_size[1] * ... * local_work_size[work_dim-1]，必须
小于或等于 CL_DEVICE_MAX_WORK_GROUP_SIZE 值。CL_DEVICE_MAX_WORK_GROUP_SIZE 的值通过 clGetDeviceInfo 查询。
local_work_size[0], ... local_work_size[work_dim-1] 中指定的工作项必须小于或等
于 CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim-1] 中对应的值。
显式的指定 local_work_size 的值将用来决定如何把 global_wok_size 指定的全局工作项进行划分为适当的工作组。
如果指定了 local_work_size 的值，global_work_size[0], ... global_work_size[1] 指定的值必须被 
local_work_size[0], ... local_work_size[1] 中对应的值整除。

> Reference:
> https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/