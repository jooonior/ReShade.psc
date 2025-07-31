;/* NOTE: These weird comments are here because this file is also included in C++ code. /;
ScriptName ReShade native
{Bindings for ReShade's addon API.}                                                   ; */

Int Property NO_RESHADE = -1 AutoReadOnly                                             ; /*
{ReShade is not loaded.}                                                              ; */

bool Function OK() global native                                                      ; /*
{Check whether ReShade is loaded.}                                                    ; */

int Function LogError(string asMessage) global native                                 ; /*
{Write `asMessage` error message to ReShade's log.}                                   ; */

int Function LogWarning(string asMessage) global native                               ; /*
{Write `asMessage` warning message to ReShade's log.}                                 ; */

int Function LogInfo(string asMessage) global native                                  ; /*
{Write `asMessage` info message to ReShade's log.}                                    ; */

int Function LogDebug(string asMessage) global native                                 ; /*
{Write `asMessage` debug message to ReShade's log.}                                   ; */
