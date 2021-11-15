## DNN with 3 features - Train, Evaluation, Draw Histograms
### 0. Presets
`conda activate mypy36`
> Conda environment with **Python3** is needed. Python3.6 recommended on HYU htop.
> 
> There is collision between PyROOT and Python3 on HYU htop, so you need to **deactivate** in order to **draw** histograms.

### 1. Train
`python train.py`
> **Train** is performed using processed ntuples in `../nanoaodframe_*LFV/` and corresponding part should be **MODIFIED**.
> 
> Training is performed with signal (LFV sample mixture) and background (tt semileptonic and dileptonic mixture) samples.
> 
> When training is done, `./<train output folder>/` will be generated (e.g. TTnov_01_norm_20nodes_3layers_s1b1) with result plots (correlation, loss, accuracy, DNN output, confusion matrix).
> 
> Best model is stored in the output folder as `best_model.h5`.

### 2. Evaluation
`python eval.py`
> **Evaluation** is performed with the best model from training and produce dnn output score histograms with data and MC samples.
> 
> To use proper model, you must check the directory of the processed ntuple used in training.
> 
> After evaluation, histogram rootfiles including "_pred" are produced inside the `./<train output folder>/pred_hists/` that generated in the train step.

### 3. Draw Histograms
`python draw.sh <train output folder>`
> Stacked histograms are drawn with `drawhists.py` but `draw.sh` will do all the process automatically.
> 
> You can change the options (systematic, years, logstyle) in `draw.sh`.
> 
> Output histograms will be stored in `./<train output folder>/pred_hists/plot_*/*.pdf`

### Advanced
> To modify some details of the plots from training, cross section for normalization, histogram styles, modules in `utils/` folder will be helpful.
