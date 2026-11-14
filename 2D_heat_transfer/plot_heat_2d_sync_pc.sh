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

mkdir ./zoomed_in_plots -p > .x

# plot execution time
gnuplot ./heat_2d_sync_tm_pc.gnu

#plot speedup using serial execution time as a reference
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt  > heat_2d_sync_MPI_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt  > heat_2d_sync_MPI_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt  > heat_2d_sync_MPI_4_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt  > heat_2d_sync_MPI_8_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt > heat_2d_sync_MPI_16_sp.txt
gnuplot ./heat_2d_sync_sp_pc.gnu

#plot efficiency using serial execution time as a reference and the specified number of processing cores
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt 1  > heat_2d_sync_MPI_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt 2 > heat_2d_sync_MPI_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt 4 > heat_2d_sync_MPI_4_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt 8 > heat_2d_sync_MPI_8_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ef.txt
gnuplot ./heat_2d_sync_ef_pc.gnu

#plot cost based on the parallel execution time and the specified number of processing cores
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_1_tm.txt  1 >  heat_2d_sync_MPI_1_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_2_tm.txt  2 >  heat_2d_sync_MPI_2_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_4_tm.txt  4 >  heat_2d_sync_MPI_4_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_8_tm.txt  8 >  heat_2d_sync_MPI_8_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ct.txt
gnuplot ./heat_2d_sync_cost_pc.gnu

#save performance data and plot outputs for this run to a new subdirectory
IDX=1
while true
do
    DIR="./sync_run_"$IDX
    if [ ! -d "$DIR" ]; then
	   mkdir $DIR
       break
    fi
    IDX=$((IDX+1))
done
echo "Moving performance data files for this run into your "$DIR" subdirectory"
mv *.txt $DIR
echo "Moving plot output files for this run into your "$DIR" subdirectory"
mv *.png $DIR
mv *.o* $DIR
mv ./zoomed_in_plots $DIR
echo "..."
echo "Creating/Updating fastest parallel execution time performance metric plots"
echo "for "$IDX" complete benchmark runs (subdirectory ./sync_bst_par_rtm)"
mkdir sync_bst_par_rtm -p > .x
/shared/wells_class/.bin/min_times heat_2d_serial_tm.txt sync_bst_par_rtm
/shared/wells_class/.bin/min_times heat_2d_sync_MPI_1_tm.txt sync_bst_par_rtm
/shared/wells_class/.bin/min_times heat_2d_sync_MPI_2_tm.txt sync_bst_par_rtm
/shared/wells_class/.bin/min_times heat_2d_sync_MPI_4_tm.txt sync_bst_par_rtm
/shared/wells_class/.bin/min_times heat_2d_sync_MPI_8_tm.txt sync_bst_par_rtm
/shared/wells_class/.bin/min_times heat_2d_sync_MPI_16_tm.txt sync_bst_par_rtm

cat heat_2d_sync_bst_tm_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_bst_par_rtm/heat_2d_sync_bst_tm_pc.gnu
cat heat_2d_sync_bst_tm_sp_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_bst_par_rtm/heat_2d_sync_bst_tm_sp_pc.gnu
cat heat_2d_sync_bst_tm_ef_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_bst_par_rtm/heat_2d_sync_bst_tm_ef_pc.gnu
cat heat_2d_sync_bst_tm_cost_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_bst_par_rtm/heat_2d_sync_bst_tm_cost_pc.gnu


rm heat_2d_sync_bst_tm_pc.gnu heat_2d_sync_bst_tm_sp_pc.gnu  heat_2d_sync_bst_tm_ef_pc.gnu heat_2d_sync_bst_tm_cost_pc.gnu 

pushd ./sync_bst_par_rtm || exit 1 > .x

mkdir ./zoomed_in_plots -p > .x

# plot execution time
gnuplot heat_2d_sync_bst_tm_pc.gnu

#plot speedup using serial execution time as a reference
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt  > heat_2d_sync_MPI_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt  > heat_2d_sync_MPI_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt  > heat_2d_sync_MPI_4_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt  > heat_2d_sync_MPI_8_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt > heat_2d_sync_MPI_16_sp.txt
gnuplot heat_2d_sync_bst_tm_sp_pc.gnu

#plot efficiency using serial execution time as a reference and the specified number of processing cores
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt 1  > heat_2d_sync_MPI_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt 2 > heat_2d_sync_MPI_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt 4 > heat_2d_sync_MPI_4_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt 8 > heat_2d_sync_MPI_8_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ef.txt
gnuplot ./heat_2d_sync_bst_tm_ef_pc.gnu

#plot cost based on the parallel execution time and the specified number of processing cores
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_1_tm.txt  1 >  heat_2d_sync_MPI_1_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_2_tm.txt  2 >  heat_2d_sync_MPI_2_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_4_tm.txt  4 >  heat_2d_sync_MPI_4_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_8_tm.txt  8 >  heat_2d_sync_MPI_8_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ct.txt
gnuplot ./heat_2d_sync_bst_tm_cost_pc.gnu

echo "..."
#**********************
popd > .x
echo "Creating/Updating slowest parallel execution time performance metric plots"
echo "for "$IDX" complete benchmark runs (subdirectory ./sync_wst_par_rtm)"
mkdir sync_wst_par_rtm -p > .x

/shared/wells_class/.bin/min_times heat_2d_serial_tm.txt sync_wst_par_rtm
/shared/wells_class/.bin/max_times heat_2d_sync_MPI_1_tm.txt sync_wst_par_rtm
/shared/wells_class/.bin/max_times heat_2d_sync_MPI_2_tm.txt sync_wst_par_rtm
/shared/wells_class/.bin/max_times heat_2d_sync_MPI_4_tm.txt sync_wst_par_rtm
/shared/wells_class/.bin/max_times heat_2d_sync_MPI_8_tm.txt sync_wst_par_rtm
/shared/wells_class/.bin/max_times heat_2d_sync_MPI_16_tm.txt sync_wst_par_rtm

cat heat_2d_sync_wst_tm_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_wst_par_rtm/heat_2d_sync_wst_tm_pc.gnu
cat heat_2d_sync_wst_tm_sp_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_wst_par_rtm/heat_2d_sync_wst_tm_sp_pc.gnu
cat heat_2d_sync_wst_tm_ef_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_wst_par_rtm/heat_2d_sync_wst_tm_ef_pc.gnu
cat heat_2d_sync_wst_tm_cost_pc.gnu | sed -e 's/_xyz_/'$IDX'/g' >./sync_wst_par_rtm/heat_2d_sync_wst_tm_cost_pc.gnu

rm heat_2d_sync_wst_tm_pc.gnu heat_2d_sync_wst_tm_sp_pc.gnu  heat_2d_sync_wst_tm_ef_pc.gnu heat_2d_sync_wst_tm_cost_pc.gnu 

mv *.gnu $DIR
pushd ./sync_wst_par_rtm || exit 1 > .x

mkdir ./zoomed_in_plots -p > .x

# plot execution time
gnuplot heat_2d_sync_wst_tm_pc.gnu

#plot speedup using serial execution time as a reference
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt  > heat_2d_sync_MPI_1_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt  > heat_2d_sync_MPI_2_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt  > heat_2d_sync_MPI_4_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt  > heat_2d_sync_MPI_8_sp.txt
/shared/wells_class/.bin/gnu_sp heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt > heat_2d_sync_MPI_16_sp.txt
gnuplot heat_2d_sync_wst_tm_sp_pc.gnu

#plot efficiency using serial execution time as a reference and the specified number of processing cores
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_1_tm.txt 1  > heat_2d_sync_MPI_1_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_2_tm.txt 2 > heat_2d_sync_MPI_2_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_4_tm.txt 4 > heat_2d_sync_MPI_4_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_8_tm.txt 8 > heat_2d_sync_MPI_8_ef.txt
/shared/wells_class/.bin/gnu_ef heat_2d_serial_tm.txt heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ef.txt
gnuplot ./heat_2d_sync_wst_tm_ef_pc.gnu

#plot cost based on the parallel execution time and the specified number of processing cores
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_1_tm.txt  1 >  heat_2d_sync_MPI_1_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_2_tm.txt  2 >  heat_2d_sync_MPI_2_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_4_tm.txt  4 >  heat_2d_sync_MPI_4_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_8_tm.txt  8 >  heat_2d_sync_MPI_8_ct.txt
/shared/wells_class/.bin/gnu_cost heat_2d_sync_MPI_16_tm.txt 16 > heat_2d_sync_MPI_16_ct.txt
gnuplot ./heat_2d_sync_wst_tm_cost_pc.gnu

echo "..."
popd > .x

