ScriptName ReShade native hidden

;//
/// ReShade is not loaded.
//;
Int Property NO_RESHADE = -1 AutoReadOnly;

;//
/// Requested ReShade runtime is not available.
//;
Int Property BAD_RUNTIME = -2 AutoReadOnly;

;//
/// Regular expression is malformed.
//;
Int Property BAD_REGEX = -3 AutoReadOnly;

;//
/// Check whether ReShade is loaded.
///
/// @returns `true` if ReShade is loaded, `false` otherwise.
///
/// @details
//;
bool Function OK() global native;

;//
/// Log an error message.
///
/// @param asMessage: The message to log.
///
/// @retval 0: The message was logged successfully.
/// @retval NO_RESHADE
///
/// @details
//;
int Function LogError(string asMessage) global native;

;//
/// Log a warning message.
///
/// @param asMessage: The message to log.
///
/// @retval 0: The message was logged successfully.
/// @retval NO_RESHADE
///
/// @details
//;
int Function LogWarning(string asMessage) global native;

;//
/// Log an info message.
///
/// @param asMessage: The message to log.
///
/// @retval 0: The message was logged successfully.
/// @retval NO_RESHADE
///
/// @details
//;
int Function LogInfo(string asMessage) global native;

;//
/// Log a debug message.
///
/// @param asMessage: The message to log.
///
/// @retval 0: The message was logged successfully.
/// @retval NO_RESHADE
///
/// @details
//;
int Function LogDebug(string asMessage) global native;

;//
/// Set state (enabled or disabled) of techniques whose name matches a regular
/// expression.
///
/// @param asTechniquePattern: Regular expression to match technique names.
/// @param abEnabled: Whether to enable (`true`) or disable (`false`) matching
///                   techniques.
/// @param aiRuntime: Target ReShade runtime.
///
/// @returns
/// Number of matching techniques whose state was set, or a negative error code.
/// @retval NO_RESHADE
/// @retval BAD_RUNTIME
/// @retval BAD_REGEX
///
/// @details
//;
int Function SetTechniqueState( \
    string asTechniquePattern, \
    bool abEnabled, \
    int aiRuntime = 0 \
) global native;

;//
/// Check whether all techniques whose names match a regular expression are in
/// a specified state (enabled or disabled).
///
/// @param asTechniquePattern: Regular expression to match technique names.
/// @param abEnabled: Whether to check for enabled (`true`) or disabled
///                   (`false`) techniques.
/// @param aiRuntime: Target ReShade runtime.
///
/// @returns
/// Number of matching techniques if all are in the specified state,
/// otherwise 0 or a negative error code.
/// @retval NO_RESHADE
/// @retval BAD_RUNTIME
/// @retval BAD_REGEX
///
/// @details
//;
int Function GetTechniqueState( \
    string asTechniquePattern, \
    bool abEnabled, \
    int aiRuntime = 0 \
) global native;

;//
/// Define a preprocessor definition.
///
/// @param asName: Name of the definition.
/// @param asValue: Value of the definition.
///
/// @retval 0: The definition was defined successfully.
/// @retval NO_RESHADE
///
/// @details
//;
int Function SetPreprocessorDefinition( \
    string asName, \
    string asValue, \
    int aiRuntime = 0 \
) global native;
