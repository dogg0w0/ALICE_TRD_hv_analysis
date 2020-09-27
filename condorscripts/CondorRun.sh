universe                = vanilla
getenv                  = true
executable              = CondorRunJob.sh
arguments               = $(sector_number) $(inputdir) $(outfile) $(date) $(outputdir)
output                  = condorOut/out.$(cluster)_$(process)
log                     = condorLog/log.$(cluster)
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT
transfer_output_files   = condoroutput

queue sector_number inputdir outfile date outputdir from sector_number.txt
