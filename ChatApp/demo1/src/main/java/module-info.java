module com.example.demo1 {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.web;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
    requires net.synedra.validatorfx;
    requires org.kordamp.ikonli.javafx;
    requires org.kordamp.bootstrapfx.core;
    requires eu.hansolo.tilesfx;
    requires com.almasb.fxgl.all;
    requires jdk.jshell;
    requires com.fasterxml.jackson.databind;
    requires java.sql;

    opens com.example.demo1.controller to javafx.fxml;
    opens com.example.demo1.domain to javafx.base;
    exports com.example.demo1;
}