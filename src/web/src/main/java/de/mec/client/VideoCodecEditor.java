/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.Label;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import com.sencha.gxt.widget.core.client.form.ConverterEditorAdapter;
import com.sencha.gxt.widget.core.client.form.FieldSet;
import com.sencha.gxt.widget.core.client.form.TextField;
import de.mec.client.model.Codec;
import de.mec.client.model.CodecProperties;
import de.mec.client.model.Video;
import de.mec.client.model.tools.CodecIdConverter;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class VideoCodecEditor extends FieldSet implements Editor<Video> {

    private static final Logger logger = Logger.getLogger(VideoEditor.class.toString());
    TextField bitrate;
    //TextField id;
    @Ignore
    public ComboBox<Codec> codecCombo;
    
    ConverterEditorAdapter<String, Codec, ComboBox<Codec>> id;

    public VideoCodecEditor() {
        setHeadingText("Basics");
        setCollapsible(true);
        Grid grid = new Grid(5, 2);
        setWidget(grid);


        //grid.setSize("200", "200");

        CodecProperties cprops = GWT.create(CodecProperties.class);

        MHiveService mhiveService = new MHiveService();

        final ListStore<Codec> codecs = new ListStore<Codec>(cprops.id());
        mhiveService.getVideoCodecs(new AsyncCallback<List<Codec>>() {
            public void onFailure(Throwable caught) {
                throw new UnsupportedOperationException("Not supported yet.");
            }

            public void onSuccess(List<Codec> result) {
                //throw new UnsupportedOperationException("Not supported yet.");
                codecs.addAll(result);
                //logger.log(Level.INFO,"CodecId:"+id.getValue());
            }
        });
        codecCombo = new ComboBox<Codec>(codecs, cprops.longName());
        codecCombo.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
        codecCombo.setForceSelection(true);
        //vlc.add(codecCombo);
        codecCombo.setWidth(400);
        id = new ConverterEditorAdapter<String, Codec, ComboBox<Codec>>(codecCombo, new CodecIdConverter(codecs));

        bitrate = new TextField();
        //bitrate.setVisible(false);
        //id = new TextField();


        grid.setWidget(0, 0, new Label("Codec Id"));
        grid.setWidget(1, 0, new Label("Video Bitrate"));

        grid.setWidget(0, 1, codecCombo);
        grid.setWidget(1, 1, bitrate);
        //grid.setWidget(2, 1, id);

    }
}
