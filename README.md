# PSO-MPI
PSO algorithm with MPI
//用于交分布式计算作业写的demo，进程通信贼尼玛多，二维0，1范围内，10个粒子用了0.6秒多才收敛完，效率高不了的，不存在的，道上的朋友们看看改进一下，就别抄了哈，我怕等下老师发现了。
//我倒是想用Grather和Bcast调用，仔细想了想这个god进程还是挺鸡肋的，其实可以改改让god别光组织排序，也参加运算这样效率可能高点，懒得写了本来效率就挺低的一个粒子怕是救不了我。
//其实改成gbest全局应该效率更高，mmp有机会再写吧。

程序的MPI实现

通过MPI生成num+1个粒子，每个粒子具有iloc，ivel，pbest，gbest属性和一个用于通讯的message[3]数组。
粒子的每个维度上的位置iloc和速度ivel在0～1范围内随机生成。

在num+1个粒子中，进程号为0的粒子被标记为全局粒子God，每个粒子更新自己的位置后将自己的pbest发送给God粒子，由God对全局排序后，将gbest发送给每个粒子。
粒子得到当前gbest之后开始新一轮的计算，直到全局收敛于目标点后停止。

PSO algorithm implemented by MPI

this demo program run with num+1(in this case,num=10) granule process,
and each granule has iloc,ivel,pbest,gbest in each dimention,and a message[3] used to IPC.

iloc,ivel initialize by a random float from 0 to 1.

the number 0 process will be signed as God process,which is used to gather other common processes' location and create now group's gbest and  broadcast this gbest.

after received,other processes will update its location until the group congregating to the objection.





