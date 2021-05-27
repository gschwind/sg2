conda env remove -n dev-py36
conda create -y -n dev-py36 python=3.6
conda activate dev-py36
pip install -r dev-requirements.txt
pip wheel . -w .\wheelhouse
conda deactivate
conda env remove -n dev-py37
conda create -y -n dev-py37 python=3.7
conda activate dev-py37
pip install -r dev-requirements.txt
pip wheel . -w .\wheelhouse
conda deactivate
conda env remove -n dev-py38
conda create -y -n dev-py38 python=3.8
conda activate dev-py38
pip install -r dev-requirements.txt
pip wheel . -w .\wheelhouse
conda deactivate
conda env remove -n dev-py39
conda create -y -n dev-py39 python=3.9
conda activate dev-py39
pip install -r dev-requirements.txt
pip wheel . -w .\wheelhouse
conda deactivate
