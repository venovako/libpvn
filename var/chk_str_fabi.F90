PROGRAM CHK_STR_FABI
  USE, INTRINSIC :: ISO_C_BINDING
  IMPLICIT NONE
  INTEGER, EXTERNAL :: CHK_STR
  CHARACTER(LEN=7), EXTERNAL :: CHK_RET
  WRITE (*,'(A,I2)') 'CHK_STR=', CHK_STR(c_char_''//c_null_char, c_char_' '//c_null_char)
  WRITE (*,'(2A)') 'CHK_RET=', CHK_RET(c_char_'foobar'//c_null_char)
END PROGRAM CHK_STR_FABI
