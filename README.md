## LFV Run2

### About Repository
> **Main features** are **two NanoAODFrames (STLFV, TTLFV)** and **DNN** environment.
> 
> Later, an initial script for compile nanoaodframes will be provided.
> 


### CMSSW Settings
```bash
echo $SCRAM_ARCH
cmsRel <CMSSW_version>
cd <CMSSW_version>/src
cmsenv
```

### Basic Workflows
> 1. Compile NanoAODFrames
> 2. Edit event selections and histograms in NanoAODFrames.
> 3. Skim NanoAOD
> 4. Process NanoAOD
> 5. Draw data/MC histograms with processed NanoAOD.
> 6. Run DNN
> 7. Run Combine

### 001. Compile
```bash
cd nanoaodframe_STLFV
make clean
make -j 4
cd ../nanoaodframe_TTLFV
make clean
make -j 4
```
