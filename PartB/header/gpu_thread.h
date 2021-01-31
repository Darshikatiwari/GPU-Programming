
#define COUNT 32 //no. of threads in each block

__global__ 
void DMM(int N, int *matA, int *matB, int *output)
{

int diag_line = blockIdx.x*blockDim.x +threadIdx.x;
int i = diag_line;

//unoptimized code
//Each thread in the given range uses this code 

if(i < N)		
{
   
    int temp = 0;
    for(int j = 0; j < i + 1; ++j)
     {
	    int rowA = j;
	    int colA = i - j;
	    int rowB = i - j;
	    int colB = N - j - 1;
	    temp += matA[rowA * N + colA] * matB[rowB * N + colB];
     }
        output[i] = temp;
}

if(i >= N && i < 2*N-1) //to prevent extra threads from executing
{
    int temp = 0;
        // Iterate over diagonal elements
    for(int j = 0; j < 2 * N - (i + 1); ++j)
    {
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
    int *GmatA,*GmatB,*GmatO;
    int bytes = N * N * sizeof(int);
    
    cudaMalloc((void**)&GmatA, bytes) ;  //memory creation in device
    cudaMalloc((void**)&GmatB, bytes) ;
    cudaMalloc((void**)&GmatO, bytes) ;
    
    cudaMemcpy(GmatA, matA, bytes, cudaMemcpyHostToDevice) ;   //data assignment
    cudaMemcpy(GmatB, matB, bytes, cudaMemcpyHostToDevice) ;
    cudaMemcpy(GmatO, output,((2*N-1)*sizeof(int)), cudaMemcpyHostToDevice) ;


    int threadsPerBlock = COUNT;
    int blocksPerGrid = (2*N) /threadsPerBlock;   //padding not required here
    
    
    dim3 blocks;
    blocks.x = blocksPerGrid;
    blocks.y = blocksPerGrid;
    blocks.z = 1;
    dim3 threads;
    threads.x = threadsPerBlock;
    threads.y = threadsPerBlock;
    threads.z = 1;
    
    DMM<<< blocks,threads >>>(N,GmatA,GmatB,GmatO);
    
    cudaMemcpy(output, GmatO, ((2*N-1)*sizeof(int)), cudaMemcpyDeviceToHost); 
     
    for(int i = 0; i < 2*N-1; ++i)
        {
            cout << "GPU actual: at : " <<i<<" : "<< output[i] << "\n";
            
        }
        
    cudaDeviceSynchronize();
    
    //cudaFree(GmatA);
    //cudaFree(GmatB);
    //cudaFree(GmatO);
}
