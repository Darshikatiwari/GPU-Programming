// Create other necessary functions here

// Fill in this function
__global__ void DMM(int N, int *matA, int *matB, int *output)
{
int i =blockIdx.x*blockDim.x +threadIdx.x;
if(i< N)
{
    //printf("%d\n",i);
    int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; ++j) {
            int rowA = j;
            int colA = i - j;
            int rowB = i - j;
            int colB = N - j - 1;
            temp += matA[rowA * N + colA] * matB[rowB * N + colB];
        }
        output[i] = temp;
}
if(i>=N && i<2*N-1)
{
    int temp = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * N - (i + 1); ++j) {
            int rowA = i + 1 + j - N;
            int colA = N - j - 1;
            int rowB = N - j - 1;
            int colB = 2 * N - j - 2 - i;
            temp += matA[rowA * N + colA] * matB[rowB * N + colB];
        }
        output[i] = temp;
}
}
void gpuThread(int N, int *matA, int *matB, int *output)
{
    int threads =16;
    int blocks = 2*N /threads;
    int *dA,*dB,*dO;
    cudaMalloc((void**)&dA, N*N*sizeof(int)) ;
    cudaMalloc((void**)&dB, N*N*sizeof(int)) ;
    cudaMalloc((void**)&dO, N*N*sizeof(int)) ;
    cudaMemcpy(dA, matA, N*N*sizeof(int), cudaMemcpyHostToDevice) ;
     cudaMemcpy(dB, matB, N*N*sizeof(int), cudaMemcpyHostToDevice) ;
      cudaMemcpy(dO, output, N*N*sizeof(int), cudaMemcpyHostToDevice) ;
    DMM<<<blocks , threads>>>(N , dA,dB ,dO);
    cudaMemcpy(matA, dA, N*N*sizeof(int), cudaMemcpyDeviceToHost); 
     cudaMemcpy(matB, dB, N*N*sizeof(int), cudaMemcpyDeviceToHost); 
      cudaMemcpy(output, dO, N*N*sizeof(int), cudaMemcpyDeviceToHost); 
     for(int i = 0; i < 256; ++i)
        {
            cout << "GPU output: at : " <<i<<" : "<< output[i] << "\n";
            
        }
    cudaDeviceSynchronize();
}
