program Blue
  use, intrinsic :: iso_fortran_env, only: output_unit, real32, real64, real128
  implicit none
  ! Blue's scaling constants from xNRM2 
  integer, parameter :: tssml = int(ceiling((minexponent(0._real32) - 1) * 0.5_real32))
  integer, parameter :: tsbig = int(floor((maxexponent(0._real32) - digits(0._real32) + 1) * 0.5_real32))
  integer, parameter :: sssml = int(-floor((minexponent(0._real32) - digits(0._real32)) * 0.5_real32))
  integer, parameter :: ssbig = int(-ceiling((maxexponent(0._real32) + digits(0._real32) - 1) * 0.5_real32))
  integer, parameter :: tdsml = int(ceiling((minexponent(0._real64) - 1) * 0.5_real64))
  integer, parameter :: tdbig = int(floor((maxexponent(0._real64) - digits(0._real64) + 1) * 0.5_real64))
  integer, parameter :: sdsml = int(-floor((minexponent(0._real64) - digits(0._real64)) * 0.5_real64))
  integer, parameter :: sdbig = int(-ceiling((maxexponent(0._real64) + digits(0._real64) - 1) * 0.5_real64))
#ifdef __GFORTRAN__
  integer, parameter :: txsml = int(ceiling((minexponent(0._10) - 1) * 0.5_10))
  integer, parameter :: txbig = int(floor((maxexponent(0._10) - digits(0._10) + 1) * 0.5_10))
  integer, parameter :: sxsml = int(-floor((minexponent(0._10) - digits(0._10)) * 0.5_10))
  integer, parameter :: sxbig = int(-ceiling((maxexponent(0._10) + digits(0._10) - 1) * 0.5_10))
#endif
  integer, parameter :: tqsml = int(ceiling((minexponent(0._real128) - 1) * 0.5_real128))
  integer, parameter :: tqbig = int(floor((maxexponent(0._real128) - digits(0._real128) + 1) * 0.5_real128))
  integer, parameter :: sqsml = int(-floor((minexponent(0._real128) - digits(0._real128)) * 0.5_real128))
  integer, parameter :: sqbig = int(-ceiling((maxexponent(0._real128) + digits(0._real128) - 1) * 0.5_real128))
  write (output_unit,*) 'SINGLE'
  write (output_unit,*) 'tsml = 2^', tssml
  write (output_unit,*) 'tbig = 2^', tsbig
  write (output_unit,*) 'ssml = 2^', sssml
  write (output_unit,*) 'sbig = 2^', ssbig
  write (output_unit,*) 'DOUBLE'
  write (output_unit,*) 'tsml = 2^', tdsml
  write (output_unit,*) 'tbig = 2^', tdbig
  write (output_unit,*) 'ssml = 2^', sdsml
  write (output_unit,*) 'sbig = 2^', sdbig
#ifdef __GFORTRAN__
  write (output_unit,*) 'EXTENDED'
  write (output_unit,*) 'tsml = 2^', txsml
  write (output_unit,*) 'tbig = 2^', txbig
  write (output_unit,*) 'ssml = 2^', sxsml
  write (output_unit,*) 'sbig = 2^', sxbig
#endif
  write (output_unit,*) 'QUAD'
  write (output_unit,*) 'tsml = 2^', tqsml
  write (output_unit,*) 'tbig = 2^', tqbig
  write (output_unit,*) 'ssml = 2^', sqsml
  write (output_unit,*) 'sbig = 2^', sqbig
end program Blue
