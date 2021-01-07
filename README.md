# ALICE_TRD_hv_analysis
Analysis code for my Bsc thesis, a more thorough explanation can be found in the thesis.

This is a short explanation of the current file structure:
~~~~
├── code
│   ├── 2017    # code to analyse the 2017 dataset 
│   │   ├── apps
│   │   │   └── src
│   │   ├── external
│   │   │   └── ROOT
│   │   └── library
│   │       ├── include
│   │       └── src
│   └── 2018    # code to analyse the 2018 dataset 
│       ├── apps
│       │   └── src
│       ├── external
│       │   └── ROOT
│       └── library
│           ├── include
│           └── src
├── condorscripts   # condorscripts to run on the kp alice condor farm
├── create_chamber_map  # this creates the chambermap to identify sorted_%i.csv.root files
├── create_csv  #   these scripts align the different dataset and produces root files to analyse
├── drfit_channel   #   some failed analyse of the drift chambers
├── errordetect #   the errordetection code
├── gain    #   code to find out if there is a significant correlation between pressure and gain
├── hv_ALICE    # source files
├── hv_ALICE_Data # source files
├── output_files    #   output files
│   ├── 2017_bare
│   ├── 2017_withGain
│   ├── 2018_bare
│   │   └── output
│   │       ├── run_0
│   │       ├── run_1
│   │       └── run_2
│   ├── 2018_noWeights
│   │   └── output
│   │       ├── run_0
│   │       ├── run_1
│   │       └── run_2
│   ├── run_2018_gain
│   │   └── output
│   │       ├── run_0
│   │       ├── run_1
│   │       └── run_2
│   └── run_2018_pressure
│       └── output
│           ├── run_0
│           ├── run_1
│           └── run_2
├── output_plots    #   output plots
│   ├── 2017_plots
│   │   ├── area
│   │   └── area_gain
│   ├── 2018_plots
│   │   ├── all
│   │   └── area
│   │       ├── run_0
│   │       ├── run_1
│   │       └── run_2
│   └── vdm
└── vdm #   code to analyse vdm scans 
~~~~