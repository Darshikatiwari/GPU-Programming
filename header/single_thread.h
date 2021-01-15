void transpose(int x, int delx, int y, int dely, int N, int* matrix) {
    if ((delx == 1) && (dely == 1)) {
        if(x<y)
        {
            int tmp = matrix[(N*y) + x];
            matrix[(N*y) + x] = matrix[(N*x) + y];
            matrix[(N*x) + y] = tmp;
        }
        return;
    }

    if (delx >= dely) {
        int xmid = delx / 2;
        transpose(x, xmid, y, dely, N, matrix);
        transpose(x + xmid, delx - xmid, y, dely, N, matrix);
        return;
    }

    int ymid = dely / 2;
    transpose(x, delx, y, ymid, N, matrix);
    transpose(x, delx, y + ymid, dely - ymid, N, matrix);
}

// void newtranspose(int x, int delx, int y, int dely, int N, int* matrix) {
//     if ((delx == x) || (dely == y)) {
//             int tmp = matrix[(N*y) + x];
//             matrix[(N*y) + x] = matrix[(N*x) + y];
//             matrix[(N*x) + y] = tmp;
//     }

//     else
//     {
//         int xmid = (x+delx) / 2;
//         int ymid = (y+dely) / 2;
//         newtranspose(x, xmid, y, ymid, N, matrix);
//         newtranspose(xmid+1, delx, y, ymid, N, matrix);
//         newtranspose(xmid+1, delx, ymid+1, dely, N, matrix);

//         if (x>=ymid+1){
//             newtranspose(x, xmid, ymid+1, dely, N, matrix);
//         }
        
//     }
// }
void mytranspose(int *dst, const int *src, int n) noexcept {
    // THROWS();
    size_t block = 64;
    for (size_t i = 0; i < n; i += block) {
        for(size_t j = 0; j < n; ++j) {
            for(size_t b = 0; b < block && i + b < n; ++b) {
                dst[j*n + i + b] = src[(i + b)*n + j];
            }
        }
    }
}

void selftranspose(int *m,size_t n)noexcept{
    size_t block=0,size=8;
    for(block=0;block+size-1<n;block+=size){
        for(size_t i=block;i<block+size;++i){
            for(size_t j=i+1;j<block+size;++j){
                std::swap(m[i*n+j],m[j*n+i]);}}
        for(size_t i=block+size;i<n;++i){
            for(size_t j=block;j<block+size;++j){
                std::swap(m[i*n+j],m[j*n+i]);}}}
    for(size_t i=block;i<n;++i){
        for(size_t j=i+1;j<n;++j){
            std::swap(m[i*n+j],m[j*n+i]);}}}

// Optimize this function
void singleThread(int N, int *matA, int *matB, int *output)
{
    auto begin = TIME_NOW;
    int *dst = new int[N * N];
    
    // transpose(0,N,0,N,N,matB);
    mytranspose(dst,matB, N);
    matB=dst;

    // selftranspose(matB,N);

    
    auto end = TIME_NOW;
    auto transposetime=(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0;
    cout << "\nTime to execute Transpose: " << transposetime << " ms\n";    
    
    // cout << "\nTranspose of Matrix B is :-\n ";
    // for(int i = 0; i < N; ++i){
    //     for(int j = 0; j < N; ++j)
    //         cout << matB[i * N + j] << "  ";
    //     cout << "\n ";
    // }

    // Iterate over first half of output elements
    // for(int i = N-1; i >= 0; i--) {
    //     int temp = 0;
    //     // Iterate over diagonal elements
    //     for(int j = 0; j < i + 1; ++j) {
    //         int t=i - j;
    //         int rowA = j;
    //         // int colA = i - j;
    //         // int rowB = i - j;
    //         int colB = N - j - 1;
    //         // temp += matA[rowA * N + colA] * matB[rowB * N + colB];
    //         temp += matA[rowA * N + t] * matB[t * N + colB];

    //         // cout << "matA[rowA * N + colA] "<< matA[rowA * N + colA];
    //         // cout << "matB[rowB * N + colB]" << matB[rowB * N + colB];
    //         // cout <<"temp = "<< temp;
    //     }
    //     output[i] = temp;
    //     // cout << output[i]<<"  ";
    // }
    
    // for(int i = N-1; i >= 0; i--) {
    //     output[i]=0;
    // }
    

    // size_t block = 32;
    // begin = TIME_NOW;
    // for(size_t i=0;i<N;i+=block){
    //     for(size_t j=0;j<N;++j){
    //         for(size_t b = 0; b < block && i + b < N; ++b) {
    //             output[i+b+j]+=matA[(i+b)*N+j]*matB[(N-(i+b)-1)*N+j];
    //         }
    //     }
    // }
    
    begin = TIME_NOW;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            output[i+j]+=matA[i*N+j]*matB[(N-i-1)*N+j];
        }
    }

    // for(int i=0;i<N;i++){
    //     int t=i*N;
    //     int p=N-i-1;
    //     for(int j=0;j<N-i;j++){
    //         output[i+j]+=matA[t+j]*matB[j*N+(p)];
    //         // cout <<"\n for i "<<i+j <<" output is  "<< output[i+j];
    //     }
    // }
    end = TIME_NOW;
    transposetime=(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0;
    cout << "\nTime to execute Loop: " << transposetime << " ms\n";    

    // for(int i = N-1; i >= 0; i--) {
    //     cout <<"\n for i "<<i <<" output is  "<< output[i];
    // }

    //  begin = TIME_NOW;
    // for(int i=N-1;i>0;i--){
    //     int t=i*N;
    //     for(int j=i-1;j>=0;j--){
    //         int p=N-j-1;
    //         output[p+i]+=matB[t+j]*matA[(p*N)+i];
    //         // cout <<"\n for i "<<i+j <<" output is  "<< output[i+j];
    //     }
    // }
    // end = TIME_NOW;
    //  transposetime=(double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0;
    // cout << "\nTime to execute second Loop: " << transposetime << " ms\n";    

    
    // for(int i = N; i < 2*N-1; i++) {
    //     cout <<"\n for i "<<i <<" output is  "<< output[i];
    // }


    // for(int i=N-1;i>0;i--){
    //     int t=i*N;
    //     for(int j=0;j<i;j++){
    //         int p=N-j-1;
    //         output[p+i]+=matB[t+j]*matA[(p*N)+i];
    //         // cout <<" for i "<<i <<" for j "<<j <<" (i-j)+i is "<<(N-j-1)+i<<" output is  "<< output[(i-j)+i] << "\n";
    //     }
    // }


    // Iterate over second half of output elements
    // int Nmult2=2*N;
    // int Nmult2minus1=Nmult2-1;
        
    // for(int i = N; i < Nmult2minus1; ++i) {
    //     int temp = 0;
    //     int iplus1=i+1;
    //     // Iterate over diagonal elements
    //     for(int j = 0; j < Nmult2 - (iplus1); ++j) {
    //         int t= N - j - 1;
    //         int rowA = iplus1 + j - N;
    //         // int colA = N - j - 1;
    //         // int rowB = N - j - 1;
    //         int colB = Nmult2 - j - 2 - i;
    //         // temp += matA[rowA * N + colA] * matB[rowB * N + colB];
    //         temp += matA[rowA * N + t] * matB[t * N + colB];
            
    //         // cout << "matA[rowA * N + colA] "<< matA[rowA * N + colA];
    //         // cout << "matB[rowB * N + colB]" << matB[rowB * N + colB];
    //         // cout <<"temp = "<< temp;
    //     }
    //     output[i] = temp;
    //     // cout << output[i] << "  ";
    // }

}
