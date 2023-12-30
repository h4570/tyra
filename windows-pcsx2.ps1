# =======================
$CUSTOM_PCSX2_PATH = "" # "D:/My/Path/To/PCSX2"
# =======================

function GetTargetELFName {
    return (Select-String -Path './Makefile' -Pattern "[^ ]*.elf").Matches.Value
}

function FindPCSX2Directory {
    if (-not [string]::IsNullOrEmpty($CUSTOM_PCSX2_PATH)) {
        return $CUSTOM_PCSX2_PATH
    }
    else {
        # Try to find in program files
        $pcsx2Path = "${Env:ProgramFiles}/PCSX2"
        $pcsx2Pathx86 = "${Env:ProgramFiles(x86)}/PCSX2"

        if (Test-Path -Path $pcsx2Path) {
            return $pcsx2Path
        }
        elseif (Test-Path -Path $pcsx2Pathx86) {
            return $pcsx2Pathx86
        }
        else {
            throw "PCSX2 directory not found!"
            return $null
        }
    }
}

function IsNewQtVersionOfPCSX2 {
    param (
        [string]$path
    )
    
    return Test-Path -Path "$path/qt.conf"
}

function FindPCSX2Executable {
    param (
        [string]$directory
    )

    $pcsx2ExePath = Join-Path -Path $directory -ChildPath 'pcsx2.exe'
    $pcsx2QtExePath = Join-Path -Path $directory -ChildPath 'pcsx2-qt.exe'

    if (Test-Path -Path $pcsx2ExePath) {
        return 'pcsx2.exe'
    }
    elseif (Test-Path -Path $pcsx2QtExePath) {
        return 'pcsx2-qt.exe'
    }
    else {
        throw "PCSX2 executable not found in: $directory!"
        return $null
    }
}

function RunPCSX2 {
    $dirPath = FindPCSX2Directory
    $isNewVersion = IsNewQtVersionOfPCSX2 -path $dirPath
    $executableName = FindPCSX2Executable -directory $dirPath
    $executableNameWithoutExt = (Split-Path $executableName -Leaf).Split('.')[0]
    $targetFileName = "$PWD/bin/$(GetTargetELFName)"

    Stop-Process -Name $executableNameWithoutExt -ErrorAction 'SilentlyContinue'

    if ($isNewVersion) {
        Start-Process -FilePath "$dirPath/$executableName" -ArgumentList "-elf", $targetFileName
    }
    else { # Old version of PCSX2
        Start-Process -FilePath "$dirPath/$executableName" -ArgumentList "--elf=$targetFileName"
    }
}

RunPCSX2