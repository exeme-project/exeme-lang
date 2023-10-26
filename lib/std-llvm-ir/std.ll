%str = type {
    i8*,    ; 0: _char_buf - pointer to the character buffer
    i64,    ; 1: length - number of characters in the buffer
}

define void @str_SEP___init__(%str* %self, i8* %string) nounwind {
    ; Initialize '_char_buf'
    %1 = getelementptr %str* %self, i64 0, i64, 0 ; get pointer to '_char_buf'
    store i8* null, i8** %1 ; store null pointer

    ; Initialize 'length'
    %2 = getelementptr %str* %self, i64 0, i64 1 ; get pointer to 'length'
    store i64 0, i64* %2 ; store 0

    ret void
}