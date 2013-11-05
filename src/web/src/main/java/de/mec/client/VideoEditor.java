/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.editor.client.CompositeEditor;
import com.google.gwt.editor.client.CompositeEditor.EditorChain;
import com.google.gwt.editor.client.EditorDelegate;
import com.google.gwt.editor.client.LeafValueEditor;
import com.google.gwt.event.logical.shared.SelectionEvent;
import com.google.gwt.event.logical.shared.SelectionHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.Label;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.core.client.dom.ScrollSupport;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.widget.core.client.Dialog;
import com.sencha.gxt.widget.core.client.button.TextButton;
import com.sencha.gxt.widget.core.client.container.FlowLayoutContainer;
import com.sencha.gxt.widget.core.client.event.SelectEvent;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import com.sencha.gxt.widget.core.client.form.ConverterEditorAdapter;
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
 * @TODO: for a dynamic ui see ->
 * http://stackoverflow.com/questions/7043760/using-gwt-editors-with-a-complex-usecase
 *
 */
public class VideoEditor extends Composite implements
        CompositeEditor<Video, Video, VideoCodecEditor>,
        LeafValueEditor<Video> /*Editor<Video>*/ {

    private Video _videoValue;
    private EditorDelegate<Video> _delegate;
    EditorChain<Video, VideoCodecEditor> _chain;
    private static final Logger logger = Logger.getLogger(VideoEditor.class.toString());

    public VideoCodecEditor createEditorForTraversal() {
        return new VideoCodecEditor();
        //throw new UnsupportedOperationException("Not supported yet."); 
    }

    public String getPathElement(VideoCodecEditor subEditor) {
        //logger.log(Level.INFO,"getting path element from subeditor : "+subEditor);
        return "";
    }

    public void setEditorChain(EditorChain<Video, VideoCodecEditor> chain) {
        //logger.log(Level.INFO,"Setting chain to : "+chain);
        //if(chain!=null)
        _chain = chain;
    }

    public void flush() {
        _videoValue = _chain.getValue(x264Editor);
        //throw new UnsupportedOperationException("Not supported yet."); 
    }

    public void onPropertyChange(String... paths) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void setValue(Video value) {
        logger.log(Level.INFO, "Setting value to : " + value.getId());
        //id.setValue(value.getId());
        _videoValue = value;
        _chain.detach(x264Editor);
        if (x264Editor != null) {
            x264Editor.removeFromParent();
        }
        if (value.getId().equals("libx264")) {
            x264Editor = new X264CodecEditor();
        } else {
            x264Editor = new VideoCodecEditor();
        }
        
        x264Editor.codecCombo.addSelectionHandler(new SelectionHandler<Codec>() {
            public void onSelection(final SelectionEvent<Codec> event) {
                
                final Dialog d = new Dialog();
                d.setHeadingText("Change Codec?");
                d.setWidget(new HTML("Change the Codec will reset all previous changes to Codec defaults!<br/><b>would you like to continue?</b>"));
                //d.setBodyStyle("fontWeight:bold;padding:13px;");
                //d.setSize("600", "400");
                //d.setResize(true);
                d.setHideOnButtonClick(true);
                d.setPredefinedButtons(Dialog.PredefinedButton.YES, Dialog.PredefinedButton.CANCEL);

                d.getButtonById(Dialog.PredefinedButton.CANCEL.name()).addSelectHandler(new SelectEvent.SelectHandler() {
                    public void onSelect(SelectEvent event) {
                        //x264Editor.codecCombo.reset();
                    }
                });
                d.getButtonById(Dialog.PredefinedButton.YES.name()).addSelectHandler(new SelectEvent.SelectHandler() {
                    public void onSelect(SelectEvent yesevent) {
                        _videoValue.setId(event.getSelectedItem().getId());
                        setValue(_videoValue);
                    }
                });
                d.show();

            }
        });
        _chain.attach(value, x264Editor);
        vlc.add(x264Editor);
    }

    public void setDelegate(EditorDelegate<Video> delegate) {
        _delegate = delegate;
        HandlerRegistration handler = _delegate.subscribe();
        logger.log(Level.INFO, "DelegateHandler:" + handler);
        //throw new UnsupportedOperationException("Not supported yet."); 
    }

    public Video getValue() {
        return _videoValue;
        //throw new UnsupportedOperationException("Not supported yet.");
    }
    //private static MyUiBinder uiBinder = GWT.create(MyUiBinder.class);
    //@Ignore
    //TextBox id;
    //@Editor.Path("")
    @Ignore
    VideoCodecEditor x264Editor;
    //@Ignore
    //ComboBox<Codec> codecCombo;
    ConverterEditorAdapter<String, Codec, ComboBox<Codec>> id;
    @Ignore
    FlowLayoutContainer vlc;

    public VideoEditor() {
        vlc = new FlowLayoutContainer();
        //vlc.setScrollSupport(ScrollSupport.ScrollMode.ALWAYS);
        //vlc.getScrollSupport().setScrollMode(ScrollSupport.ScrollMode.AUTO);

        vlc.setScrollMode(ScrollSupport.ScrollMode.AUTO);
        //VBoxLayoutContainer vbox = new VBoxLayoutContainer();
        //vbox.setVBoxLayoutAlign(VBoxLayoutAlign.STRETCH);
        //vbox.setPadding(new Padding(5));
        //Grid grid = new Grid(2, 2);

        //grid.setSize("400", "400");
        //vbox.setSize("400","400");

        //vbox.add(grid,new BoxLayoutData(new Margins(0, 0, 5, 0)));
        this.initWidget(vlc);
        //vlc.add(grid);
        //setWidget(vbox);



        //Label lblAge = new Label("Video Codec");
        //grid.setWidget(0, 0, lblAge);

        /*
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
            }
        });*/
        /*
        codecCombo = new ComboBox<Codec>(codecs, cprops.longName());
        codecCombo.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
        codecCombo.setForceSelection(true);
        vlc.add(codecCombo);
        codecCombo.setWidth(400);
        id = new ConverterEditorAdapter<String, Codec, ComboBox<Codec>>(codecCombo, new CodecIdConverter(codecs));
        * */
        //grid.setWidget(0, 1, codecCombo);
        //grid.setWidget(0,1,codecCombo);

        //x264Editor = new VideoCodecEditor();
        //x264Editor.setVisible(false);
        //vlc.add(x264Editor);
        //grid.setWidget(1,1, x264Editor);
        //vbox.forceLayout();
        //vlc.forceLayout();
        vlc.setSize("100%", "200");
        /*
        codecCombo.addSelectionHandler(new SelectionHandler<Codec>() {
            public void onSelection(SelectionEvent<Codec> event) {
                //flush();
                _videoValue.setId(event.getSelectedItem().getId());
                setValue(_videoValue);
                logger.log(Level.INFO, "response data :" + event.getSelectedItem().getId());
            }
        });*/
    }
}
