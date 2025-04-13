import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import turism.client.gui.LoginController;
import turism.client.gui.UserController;
import turism.network.rpcprotocol.TurismServicesRpcProxy;
import turism.services.ITurismServices;

import java.io.File;
import java.io.IOException;
import java.util.Properties;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main extends Application {
    private Stage primaryStage;

    private static int defaultPort = 55555;
    private static String defaultServer = "localhost";

    private static Logger logger = LogManager.getLogger(Main.class);

    public void start(Stage primaryStage) throws Exception {
        logger.debug("Starting client application");
        Properties clientProps = new Properties();
        try{
            clientProps.load(Main.class.getResourceAsStream("/turismclient.properties"));
            logger.info("Client properties set {}", clientProps);
        } catch (IOException e){
            logger.error("Cannot find turismclient.properties {}", e);
            logger.debug("Looking into folder {}", new File(".").getAbsolutePath());
        }
        String serverIP = clientProps.getProperty("turism.server.host", defaultServer);
        int serverPort = defaultPort;

        try{
            serverPort = Integer.parseInt(clientProps.getProperty("turism.server.port"));
        } catch (NumberFormatException e){
            logger.error("Wrong port number {}", clientProps.getProperty("turism.server.port"));
            logger.debug("Using default port {}", defaultPort);
        }
        logger.info("Connecting to server {} on port {}", serverIP, serverPort);

        ITurismServices server = new TurismServicesRpcProxy(serverIP, serverPort);

        FXMLLoader loader = new FXMLLoader(
                getClass().getClassLoader().getResource("LoginView.fxml"));
        Parent root = loader.load();
        LoginController ctrl = loader.getController();
        ctrl.setServer(server);

        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("UserView.fxml"));
        Parent rootUser = fxmlLoader.load();
        UserController ctrlUser = fxmlLoader.getController();
        ctrlUser.setServer(server);

        ctrl.setUserController(ctrlUser);
        ctrl.setParent(rootUser);

        primaryStage.setTitle("Login");
        primaryStage.setScene(new Scene(root));
        primaryStage.show();
    }
}