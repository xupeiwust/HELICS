function v = HELICS_HANDLE_OPTION_CONNECTIONS()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 119);
  end
  v = vInitialized;
end
