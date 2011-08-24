// Shellsort: sort first N items in array A
// Etype: must have copy constructor, operator=, and operator<

template <class Etype>
void
ShellSort( Etype A[ ], int N )
{
    for( int Gap = N / 2; Gap > 0; Gap = Gap == 2 ? 1 :
			( int ) ( Gap / 2.2 ) )
        for( int i = Gap; i < N; i++ )
        {
            Etype Tmp = A[ i ];
            int j = i;

            for( ; j >= Gap && Tmp < A[ j - Gap ]; j -= Gap )
                    A[ j ] = A[ j - Gap ];
            A[ j ] = Tmp;
        }
}
