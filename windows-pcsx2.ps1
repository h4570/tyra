$PCSX2_PATH = "${Env:ProgramFiles(x86)}/PCSX2"
$PCSX2_EXE = 'pcsx2.exe'

function GetTargetELFName {
    return (Select-String -Path './Makefile' -Pattern "[^ ]*.elf").Matches.Value
}

function RunPCSX2 {
    $PCSX2_EXE_WITHOUT_EXT = (Split-Path $PCSX2_EXE -Leaf).Split('.')[0]
    Stop-Process -Name $PCSX2_EXE_WITHOUT_EXT -ErrorAction 'SilentlyContinue'
    Start-Process -FilePath "$PCSX2_PATH/$PCSX2_EXE" -ArgumentList "--elf=$PWD/bin/$(GetTargetELFName)"
}