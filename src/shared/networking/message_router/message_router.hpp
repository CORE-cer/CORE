namespace CORE::Internal {
class MessageRouter {
  /**
   * An interface for routing incomming messages. Note that this
   * interface does not use virtual methods because it is intented to be
   * used in metetemplating to avoid dereferenciation as it will be used
   * repeatedly in the program.
   */
  // There needs to be an initialization that specifies the address and the
  // action.
  void start();
  void stop();
};
}  // namespace CORE::Internal
