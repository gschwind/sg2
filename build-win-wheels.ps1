
$pyvers="38","39","310","311"
foreach($ver in $pyvers) {
	if (Test-Path -LiteralPath "dev-py$ver") {
		Remove-Item -LiteralPath "dev-py$ver" -Force -Recurse
	}
	Start-Process -Wait -NoNewWindow -FilePath "C:\Python$ver\python.exe" -ArgumentList '-m','venv',"dev-py$ver"
	Start-Process -Wait -NoNewWindow -FilePath "$PSScriptRoot\dev-py$ver\Scripts\python.exe" -ArgumentList '-m','pip',"install","-U","pip"
	. "$PSScriptRoot\dev-py$ver\Scripts\Activate.ps1"
	pip wheel . -w .\wheelhouse
}

