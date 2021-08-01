

__global__ void sinewave_kernel(float4 *pos,unsigned int width, unsigned int height,float time)
{
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

    float u = i / (float)width;
    float v = j / (float)height;
    u = u * 2.0f - 1.0f;
    v = v * 2.0f - 1.0f;
    float frequecy = 4.0f;
    float w = sinf(u * frequecy + time) * cosf(v * frequecy + time) * 0.5f;

    pos[y * width + x] = make_float4(u,w,v,1.0f);

    void LaunchCUDAKernel(float4 *pos,unsigned int width, unsigned int height,float time)
    {
        dim3 block(8,8,1);
        dim3 grid(width/block.x,height/block.y,1);
        sinewave_kernel<<<grid,block>>>(pos,width,height,time);
    }
}
