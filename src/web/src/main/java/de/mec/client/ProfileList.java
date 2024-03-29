/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.client.GWT;
import com.google.gwt.editor.client.Editor;
import com.google.gwt.editor.client.SimpleBeanEditorDriver;
import com.google.gwt.event.logical.shared.SelectionEvent;
import com.google.gwt.event.logical.shared.SelectionHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.sencha.gxt.core.client.ValueProvider;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.data.shared.ModelKeyProvider;
import com.sencha.gxt.data.shared.PropertyAccess;
import com.sencha.gxt.widget.core.client.ContentPanel;
import com.sencha.gxt.widget.core.client.Dialog;
import com.sencha.gxt.widget.core.client.Dialog.PredefinedButton;
import com.sencha.gxt.widget.core.client.button.TextButton;
import com.sencha.gxt.widget.core.client.button.ToolButton;
import com.sencha.gxt.widget.core.client.event.CellDoubleClickEvent;
import com.sencha.gxt.widget.core.client.event.SelectEvent;
import com.sencha.gxt.widget.core.client.event.SelectEvent.SelectHandler;
import com.sencha.gxt.widget.core.client.grid.ColumnConfig;
import com.sencha.gxt.widget.core.client.grid.ColumnModel;
import com.sencha.gxt.widget.core.client.grid.Grid;
import com.sencha.gxt.widget.core.client.menu.Menu;
import com.sencha.gxt.widget.core.client.menu.MenuItem;
import de.mec.client.model.Profile;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class ProfileList extends ContentPanel {

    private static final Logger logger = Logger.getLogger(ProfileList.class.toString());
    final MHiveService mhiveService;

    interface ProfileProperties extends PropertyAccess<Profile> {

        @Editor.Path("uuid")
        ModelKeyProvider<Profile> key();

        ValueProvider<Profile, String> uuid();

        ValueProvider<Profile, String> name();
    }

    public ProfileList() {
        //logger.log(Level.SEVERE, "loading ProfileList");
        setHeadingText("Profile List");
        ToolButton plus = new ToolButton(ToolButton.PLUS);
        plus.addSelectHandler(new SelectHandler() {
            public void onSelect(SelectEvent event) {
                Profile profile = mhiveService.createProfile();
                openEditor(profile);
            }
        });
        addTool(plus);


        ProfileProperties props = GWT.create(ProfileProperties.class);

        mhiveService = new MHiveService();
        final ListStore<Profile> store = new ListStore<Profile>(props.key());
        mhiveService.getProfiles(new AsyncCallback<List<Profile>>() {
            public void onFailure(Throwable caught) {
                // logger.log(Level.SEVERE, "exception :" + caught);
            }

            public void onSuccess(List<Profile> result) {
                //logger.log(Level.INFO, "returned result :" + result);
                store.addAll(result);
            }
        });


        ColumnConfig<Profile, String> cc1 = new ColumnConfig<Profile, String>(props.uuid(), 100, "UUID");
        ColumnConfig<Profile, String> cc2 = new ColumnConfig<Profile, String>(props.name(), 165, "Name");

        List<ColumnConfig<Profile, ?>> l = new ArrayList<ColumnConfig<Profile, ?>>();
        l.add(cc1);
        l.add(cc2);
        ColumnModel<Profile> cm = new ColumnModel<Profile>(l);

        //ProfileAutoBeanFactory factory = GWT.create(ProfileAutoBeanFactory.class);
        //DataRecordJsonReader reader = new DataRecordJsonReader(factory, RecordResult.class);

        //RequestBuilder builder = new RequestBuilder(RequestBuilder.GET, "/api/v1/profile/");
        //HttpProxy<ListLoadConfig> proxy = new HttpProxy<ListLoadConfig>(builder);
        //final ListLoader<ListLoadConfig, ListLoadResult<Profile>> loader = new ListLoader<ListLoadConfig, ListLoadResult<Profile>>(proxy, reader);
        //loader.useLoadConfig(factory.create(ListLoadConfig.class).as());
        //ListStore<Profile> store = new ListStore<Profile>(props.key());
        //loader.addLoadHandler(new LoadResultListStoreBinding<ListLoadConfig, Profile, ListLoadResult<Profile>>(store));


        Grid<Profile> grid = new Grid<Profile>(store, cm);

        grid.getView().setForceFit(true);
        //grid.setLoader(loader);
        grid.setLoadMask(true);
        grid.setBorders(true);
        grid.getView().setEmptyText("Please hit the load button.");
        //loader.load();
        add(grid);
        /*
         * grid.addListener(Events.CellDoubleClick, new
         * Listener<GridEvent<Profile>>() {
         *
         * @Override public void handleEvent(GridEvent<Employee> be) {
         * System.out.println(be.getModel().getEmployeeName());
         *
         * }
         *
         * });
         */

        grid.addCellDoubleClickHandler(new CellDoubleClickEvent.CellDoubleClickHandler() {
            public void onCellClick(CellDoubleClickEvent event) {
                Profile profile = (Profile) event.getSource().getSelectionModel().getSelectedItem();
                logger.log(Level.INFO, "double click data :" + profile.getUuid());
                openEditor(profile);
            }
        });


        Menu contextMenu = new Menu();
        contextMenu.setWidth(140);

        MenuItem insert = new MenuItem();
        insert.setText("Create Encoding");
        contextMenu.add(insert);

        MenuItem remove = new MenuItem();
        remove.setText("Remove Selected");
        //remove.setIcon(null);  
        contextMenu.add(remove);

        insert.addSelectionHandler(new SelectionHandler() {
            public void onSelection(SelectionEvent event) {
                
            }
        });
        grid.setContextMenu(contextMenu);
    }

    private void openEditor(final Profile profile) {
        final Driver driver = GWT.create(Driver.class);
        final Dialog d = new Dialog();
        d.setHeadingText("Edit Profile!");
        //d.setBodyStyle("fontWeight:bold;padding:13px;");
        d.setSize("800", "600");
        d.setResize(true);
        //d.setHideOnButtonClick(true);
        d.setPredefinedButtons(PredefinedButton.YES, PredefinedButton.CANCEL);
        TextButton but = d.getButtonById(PredefinedButton.YES.name());

        d.getButtonById(PredefinedButton.CANCEL.name()).addSelectHandler(new SelectEvent.SelectHandler() {
            public void onSelect(SelectEvent event) {
                if (!driver.isDirty()) {
                    d.hide();
                }
            }
        });
        but.addSelectHandler(new SelectEvent.SelectHandler() {
            public void onSelect(SelectEvent event) {
                logger.log(Level.INFO, "OK Button clicked");
                Profile p = driver.flush();
                logger.log(Level.INFO, "editor profile :" + p.getName());
                mhiveService.saveProfile(p, new AsyncCallback<Profile>() {
                    public void onFailure(Throwable caught) {
                        logger.log(Level.WARNING, "editor profile save:", caught);
                        throw new IllegalStateException(caught);
                    }

                    public void onSuccess(Profile result) {
                        logger.log(Level.INFO, "editor profile save: " + result);
                        d.hide();

                    }
                });
            }
        });
        d.show();

        final ProfileEditor editor = new ProfileEditor();
        d.add(editor);
        driver.initialize(editor);
        //d.show();
        mhiveService.getProfile(profile.getUuid(), new AsyncCallback<Profile>() {
            public void onFailure(Throwable caught) {
                logger.log(Level.INFO, "profile data not found");
                driver.edit(profile);
            }

            public void onSuccess(Profile result) {

                logger.log(Level.INFO, "profile data :" + result.getVideoMap());
                driver.edit(result);
                /*
                 result.getVideoMap().put("bla", "test");
                 //d.setProfile(result);
            
                 logger.log(Level.INFO, "profile data :" + result.getVideoMap().keySet());
                 for(String key:result.getVideoMap().keySet()){
                 logger.log(Level.INFO, "profile data video.key :" + key);
                 }*/
            }
        });

    }

    interface Driver extends SimpleBeanEditorDriver<Profile, ProfileEditor> {
    }
}
