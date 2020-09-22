# analysis_run
add_executable( analysis_run )
target_sources( analysis_run PRIVATE "src/analysis_run.cpp" )
target_link_libraries( analysis_run PUBLIC DictROOT )