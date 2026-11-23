#!/bin/bash
# script file to plot execution time and the other derived performance metrics
# of speedup, efficiency, and cost using gnuplot
# After ALL jobs have successfully executed this script can be applied
# Produces .png files which can be downloaded and viewed on client systems
sq=`squeue`
if [[ $sq == *"$USER"* ]]; then
   echo "Elements still in the SLURM Queue"
   echo "Need to clear SLURM Queue before creating plots"
   squeue
   exit
fi

# plot execution time
gnuplot ./heat_2d_MPI_OMP_1_tm_pc.gnu
gnuplot ./heat_2d_MPI_OMP_2_tm_pc.gnu
gnuplot ./heat_2d_MPI_OMP_4_tm_pc.gnu
gnuplot ./heat_2d_MPI_OMP_8_tm_pc.gnu
gnuplot ./heat_2d_MPI_OMP_16_tm_pc.gnu

#plot speedup using serial execution time as a reference
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_1_tm.txt  > heat_2d_MPI_OMP_1_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_2_tm.txt  > heat_2d_MPI_OMP_1_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_3_tm.txt  > heat_2d_MPI_OMP_1_3_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_4_tm.txt  > heat_2d_MPI_OMP_1_4_sp.txt
gnuplot ./heat_2d_MPI_OMP_1_sp_pc.gnu

/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_1_tm.txt  > heat_2d_MPI_OMP_2_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_2_tm.txt  > heat_2d_MPI_OMP_2_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_3_tm.txt  > heat_2d_MPI_OMP_2_3_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_4_tm.txt  > heat_2d_MPI_OMP_2_4_sp.txt
gnuplot ./heat_2d_MPI_OMP_2_sp_pc.gnu

/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_1_tm.txt  > heat_2d_MPI_OMP_4_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_2_tm.txt  > heat_2d_MPI_OMP_4_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_3_tm.txt  > heat_2d_MPI_OMP_4_3_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_4_tm.txt  > heat_2d_MPI_OMP_4_4_sp.txt
gnuplot ./heat_2d_MPI_OMP_4_sp_pc.gnu

/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_1_tm.txt  > heat_2d_MPI_OMP_8_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_2_tm.txt  > heat_2d_MPI_OMP_8_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_3_tm.txt  > heat_2d_MPI_OMP_8_3_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_4_tm.txt  > heat_2d_MPI_OMP_8_4_sp.txt
gnuplot ./heat_2d_MPI_OMP_8_sp_pc.gnu

/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_1_tm.txt  > heat_2d_MPI_OMP_16_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_2_tm.txt  > heat_2d_MPI_OMP_16_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_3_tm.txt  > heat_2d_MPI_OMP_16_3_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_4_tm.txt  > heat_2d_MPI_OMP_16_4_sp.txt
gnuplot ./heat_2d_MPI_OMP_16_sp_pc.gnu


#plot efficiency using serial execution time as a reference and the specified number of processing cores
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_1_tm.txt  1 > heat_2d_MPI_OMP_1_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_2_tm.txt  2 > heat_2d_MPI_OMP_1_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_3_tm.txt  3 > heat_2d_MPI_OMP_1_3_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_1_4_tm.txt  4 > heat_2d_MPI_OMP_1_4_ef.txt
gnuplot ./heat_2d_MPI_OMP_1_ef_pc.gnu

/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_1_tm.txt 2 > heat_2d_MPI_OMP_2_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_2_tm.txt 4 > heat_2d_MPI_OMP_2_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_3_tm.txt 6 > heat_2d_MPI_OMP_2_3_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_2_4_tm.txt 8 > heat_2d_MPI_OMP_2_4_ef.txt
gnuplot ./heat_2d_MPI_OMP_2_ef_pc.gnu

/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_1_tm.txt 4 > heat_2d_MPI_OMP_4_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_2_tm.txt 8 > heat_2d_MPI_OMP_4_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_3_tm.txt 12 > heat_2d_MPI_OMP_4_3_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_4_4_tm.txt 16 > heat_2d_MPI_OMP_4_4_ef.txt
gnuplot ./heat_2d_MPI_OMP_4_ef_pc.gnu

/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_1_tm.txt  8 > heat_2d_MPI_OMP_8_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_2_tm.txt 16 > heat_2d_MPI_OMP_8_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_3_tm.txt 24 > heat_2d_MPI_OMP_8_3_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_8_4_tm.txt 32 > heat_2d_MPI_OMP_8_4_ef.txt
gnuplot ./heat_2d_MPI_OMP_8_ef_pc.gnu

/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_1_tm.txt 16 > heat_2d_MPI_OMP_16_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_2_tm.txt 32 > heat_2d_MPI_OMP_16_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_3_tm.txt 48 > heat_2d_MPI_OMP_16_3_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_MPI_OMP_16_4_tm.txt 64 > heat_2d_MPI_OMP_16_4_ef.txt
gnuplot ./heat_2d_MPI_OMP_16_ef_pc.gnu

#save performance data and plot outputs for this run to a new subdirectory
IDX=1
while true
do
    DIR="./MPI_OMP_run_"$IDX
    if [ ! -d "$DIR" ]; then
	   mkdir $DIR
       break
    fi
    IDX=$((IDX+1))
done
echo "Moving performance data files for this run into your "
echo "   "$DIR" subdirectory"
mv *.txt $DIR
echo "Moving plot output files for this run into your "
echo "   "$DIR" subdirectory"
mv *.png $DIR
echo "Moving batch script output files for this run into your "
echo "   "$DIR" subdirectory"
mv heat2d*.o* $DIR
