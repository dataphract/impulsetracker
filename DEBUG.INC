;
; Debug macro. To write to the file, use "Trace <logmessage>"
;

IF TRACEENABLED

IF CREATENEWLOGFILE
FirstTime       DB      0
ENDIF

LogFileName     DB      "Logfile.Txt", 0

;───────────────────────────────────────────────────────────────────────────────

Proc            WriteDebugFile

                PushA
                Push    DS

                Push    CS
                Pop     DS

                Push    CX
                Mov     DX, Offset LogFileName

IF CREATENEWLOGFILE

                  Cmp     DS:FirstTime, 0
                  JNE     WriteDebugFile1

                  Mov     AH, 3Ch
                  Xor     CX, CX
                  Int     21h
                  JC      WriteDebugFileEnd

                  Mov     DS:FirstTime, 1
                  XChg    AX, BX
                  Jmp     WriteDebugFile2

  WriteDebugFile1:

ENDIF
                Mov     AX, 3D02h
                Int     21h
                JC      WriteDebugFileEnd

                XChg    AX, BX

                Mov     AX, 4202h
                Xor     CX, CX
                Xor     DX, DX
                Int     21h             ; Move to end of file

WriteDebugFile2:
                Mov     AH, 40h
                Pop     CX
                Push    CX
                Mov     DX, SI
                Int     21h

                Mov     AH, 3Eh
                Int     21h

WriteDebugFileEnd:
                Pop     CX
                Pop     DS
                PopA
                Ret

EndP            WriteDebugFile

;───────────────────────────────────────────────────────────────────────────────

Trace           Macro LogMessage
                Local X, Y

                PushF
                Push    CX
                Push    SI
                Jmp     Y
        X:
                DB      LogMessage
                DB      0Dh, 0Ah
        Y:
                Mov     CX, Offset Y - Offset X
                Mov     SI, Offset X
                Call    WriteDebugFile

                Pop     SI
                Pop     CX
                PopF

                EndM

;───────────────────────────────────────────────────────────────────────────────

ELSE

;───────────────────────────────────────────────────────────────────────────────

Trace           Macro LogMessage
                EndM

;───────────────────────────────────────────────────────────────────────────────

ENDIF

