$Name = "aimnet"

.\environment.ps1

pip install opencv-python torch torchvision

conda install -n $Name matplotlib pywin32 ipython `
	Pillow PyYAML requests scipy pandas tqdm seaborn -y
