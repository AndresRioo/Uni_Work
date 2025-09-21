module Server {
    requires ComUtils;
    requires java.logging;  // Añade esta línea para acceder a java.util.logging

    exports p1.server;
}
