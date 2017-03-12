#!/bin/sh
# $Id$
##################################################################
# This script defines a set of sample simulation cases that
# is used to verify the integrity of an IFEM-based simulator.
# Copy this file to a sub-folder Test in your App-directory and
# insert the simulations that you want to use as regression tests.
##################################################################

# Define the name of the executable here
mysim=LinEl

run () {
# This function runs a simulation with the specified options,
# pipes the terminal output to a log-file, and compares it
# with a previous simulation stored in the Reference folder.
  if `echo $2 | grep -q '^\-'`; then
    inp=$1
    log=`basename $1 .inp`.log
  else
    inp=$2.inp
    log=$2.log
    ln -s $1 $inp
    shift
  fi
  echo Running $inp ...
  shift
  time -f "real time %E, user time %U, system time %S" \
  ../../../Release/bin/$mysim $inp $* > $log
  if [ -h $inp ]; then rm $inp; fi
  if [ ! -e Reference/$log ]; then
    mv $log Reference
  elif cmp -s $log Reference/$log; then
    echo Ok
  else
    echo "Warning: Discrepancies between current run and reference."
    diff $log Reference
  fi
}

if [ ! -d Reference ]; then mkdir Reference; fi

#####################################
# Enter the various cases below:
# Format: run <inputfile> [<options>]
#####################################

run Hole2D-p3.inp Hole2D-NURBS    -2Dpstrain -vtf 1 -nviz 4 -hdf5
run Hole2D-p3.inp Hole2D-Lagrange -2Dpstrain -vtf 1 -lagrange
run Hole2D-p3.inp Hole2D-Spectral -2Dpstrain -vtf 1 -spectral

run Cylinder-p4.inp Cylinder-NURBS    -checkRHS -vtf 1 -nviz 5 -hdf5
run Cylinder-p4.inp Cylinder-Lagrange -checkRHS -vtf 1 -lagrange -nGauss 5
run Cylinder-p4.inp Cylinder-Spectral -checkRHS -vtf 1 -spectral

run Cylinder-Axisymm.inp -2Daxisymm -vtf 1 -nviz 5 -hdf5

eigOpt="-eig 5 -nev 20 -ncv 40 -shift 2.0e-4"
run SquarePlate-p2.inp SquarePlate-Splines  $eigOpt -nGauss 3 -vtf 1 -nviz 3
run SquarePlate-p2.inp SquarePlate-Lagrange $eigOpt -nGauss 3 -vtf 1 -lagrange
run SquarePlate-p2.inp SquarePlate-Spectral $eigOpt -nGauss 3 -vtf 1 -spectral

run PipeJoint.inp PipeJoint-vibration -vtf 1 -nu 7 -nv 7 -free -eig 4 -nev 16 -ncv 32
run PipeJoint.inp PipeJoint-NURBS     -vtf 1 -nu 7 -nv 7 -hdf5
run PipeJoint.inp PipeJoint-Lagrange  -vtf 1 -lagrange

run NavierPress_p2.inp -2DKL -nGauss 2 -vtf 1 -nviz 7
run NavierPress_p3.inp -2DKL -nGauss 3 -vtf 1 -nviz 7 -hdf5
run NavierPart_p2.inp  -2DKL -nGauss 2 -vtf 1 -nviz 7
run NavierPart_p3.inp  -2DKL -nGauss 3 -vtf 1 -nviz 7 -hdf5
run NavierPoint_p3.inp -2DKL -nGauss 3 -vtf 1 -nviz 5 -hdf5
