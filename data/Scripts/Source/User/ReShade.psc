;/* NOTE: These weird comments are here because this file is also included in C++ code. /;
ScriptName ReShade native
{Bindings for ReShade's addon API.}                                                   ; */

Int Property NO_RESHADE = -1 AutoReadOnly                                             ; /*
{ReShade is not loaded.}                                                              ; */

Int Property BAD_RUNTIME = -2 AutoReadOnly                                            ; /*
{Requested runtime is not available.}                                                 ; */

Int Property BAD_REGEX = -3 AutoReadOnly                                              ; /*
{Invalid regular expression.}                                                         ; */

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

int Function SetTechniqueState(string asTechniquePattern, bool abEnabled, int aiRuntime = 0) global native  ; /*
{Enable/disable techniques whose names match specified regular expression.}                                 ; */
