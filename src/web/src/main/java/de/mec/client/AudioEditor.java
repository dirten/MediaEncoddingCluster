/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.editor.client.Editor;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.SimplePanel;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.widget.core.client.Slider;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import com.sencha.gxt.widget.core.client.form.ConverterEditorAdapter;
import com.sencha.gxt.widget.core.client.form.TextField;
import de.mec.client.model.Audio;
import de.mec.client.model.Codec;
import de.mec.client.model.CodecProperties;
import de.mec.client.model.tools.CodecIdConverter;
import de.mec.client.model.tools.IntegerStringConverter;
import java.util.List;

/**
 *
 * @author jan.hoelscher
 */
public class AudioEditor extends SimplePanel implements Editor<Audio> {

    @Ignore
    ComboBox<Codec> codecCombo;
    
    ConverterEditorAdapter<String, Codec, ComboBox<Codec>> id;
    
    @Ignore
    TextField sampleRateField;
    
    ConverterEditorAdapter<Integer, String, TextField> sampleRate;
    
    Slider bitrate;

    public AudioEditor() {
        Grid grid = new Grid(4, 3);
        setWidget(grid);
        grid.setSize("200", "200");

        Label lblAge = new Label("Audio Codec");
        grid.setWidget(0, 0, lblAge);
        CodecProperties cprops = GWT.create(CodecProperties.class);

        MHiveService mhiveService = new MHiveService();

        final ListStore<Codec> codecs = new ListStore<Codec>(cprops.id());
        mhiveService.getAudioCodecs(new AsyncCallback<List<Codec>>() {
            public void onFailure(Throwable caught) {
                throw new UnsupportedOperationException("Not supported yet.");
            }
            public void onSuccess(List<Codec> result) {
                codecs.addAll(result);
            }
        });
        codecCombo = new ComboBox<Codec>(codecs, cprops.longName());
        codecCombo.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
        codecCombo.setForceSelection(true);
        codecCombo.setWidth(200);
        id = new ConverterEditorAdapter<String, Codec, ComboBox<Codec>>(codecCombo, new CodecIdConverter(codecs));
        grid.setWidget(0, 1, codecCombo);
        
        
        sampleRateField=new TextField();
        sampleRateField.setWidth(200);
        grid.setWidget(1,1, sampleRateField);
        grid.setWidget(1,0,new Label("Audio Samplerate"));
        sampleRate = new ConverterEditorAdapter<Integer, String, TextField>(sampleRateField, new IntegerStringConverter());
        
        
        
        bitrate=new Slider();
        bitrate.setMaxValue(1000000);
        bitrate.setMinValue(1);
        grid.setWidget(2,0,new Label("Audio Bitrate"));
        grid.setWidget(2,1,bitrate);
        
        /*
        final TextField sliderValue=new TextField();
        grid.setWidget(2,2,sliderValue);
        bitrate.addValueChangeHandler(new ValueChangeHandler<Integer>() {
            public void onValueChange(ValueChangeEvent<Integer> event) {
                sliderValue.setText(event.getValue().toString());
            }
        });
        */
    }
}
