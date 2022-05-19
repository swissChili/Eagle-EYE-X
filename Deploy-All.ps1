$Deps = @(
	"msvcp140.dll",
	"msvcp140_1.dll",
	"vcruntime140.dll",
	"vcruntime140_1.dll",
	"vcomp140.dll"
)

function Create-DirIfNotExists {
	param ( $Name )

	if (! (Test-Path -Path $Name)) {
		mkdir $Name
	}
}

function Print-Lines {
	param ( $Message )
	echo ""
	echo ==========================================
	echo "👀 $Message"
	echo ==========================================
}

$CurrentDir = (pwd).Path
$DeployPath = "$CurrentDir\Deploy"

Create-DirIfNotExists $DeployPath

$OutWeightsPath = "$DeployPath\model_csgo.weights"

Print-Lines "Building Visual Studio projects"
$env:Platform = "X64"
msbuild /m /p:Configuration=Release .\AiMNET.sln

if (! (Test-Path -Path $OutWeightsPath)) {
	cp ".\InferenceEngine\Data\yolov4.weights" $OutWeightsPath
}

Print-Lines "Copying InferenceEngine.exe dependencies"

foreach ($Dep in $Deps) {
	if (! (Test-Path -Path $DeployPath\$Dep)) {
		cp C:\Windows\System32\$Dep $DeployPath
	}
}

cp ".\Build\Release\InferenceEngine\InferenceEngine.exe" $DeployPath
cp ".\Build\Release\InferenceEngine\DirectML.dll" $DeployPath

Print-Lines "Copying compiled shaders"

cp .\Build\Release\InferenceEngine\*.cso $DeployPath

# Build client

Create-DirIfNotExists Build
Create-DirIfNotExists Build\Client

$ClientBuildPath = "$CurrentDir\Build\Client"
$ClientPro = "$CurrentDir\Client\Eagle-EYE-X.pro"
$ClientQmlDir = "$CurrentDir\Client\"

echo $ClientPro

cd $ClientBuildPath
qmake $ClientPro

Print-Lines Building Client

nmake

Print-Lines "Deploying Client"

cd $DeployPath
cp "$ClientBuildPath\release\Eagle-EYE-X.exe" .
windeployqt Eagle-EYE-X.exe --qmldir $ClientQmlDir

rm vc_redist.x64.exe

Print-Lines "DONE ✅
All files built to $DeployPath"

cd $CurrentDir