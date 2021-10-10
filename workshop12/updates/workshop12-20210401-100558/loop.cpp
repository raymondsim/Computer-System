// sum length elements of data in the order specified by indices
int iterate(int *data,int *indices,int length)
{
    int x = 0 ;

    for ( int i = 0 ; i < length ; i++ )
    {
        x += data[indices[i]] ;
    }

    return x ;
}

