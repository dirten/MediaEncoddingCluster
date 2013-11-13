/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.editor.client.CompositeEditor;
import com.google.gwt.editor.client.EditorDelegate;
import com.google.gwt.editor.client.ValueAwareEditor;
import com.google.gwt.event.logical.shared.SelectionEvent;
import com.google.gwt.event.logical.shared.SelectionHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.SimplePanel;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.core.client.dom.ScrollSupport;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.widget.core.client.container.FlowLayoutContainer;
import com.sencha.gxt.widget.core.client.container.VerticalLayoutContainer;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import com.sencha.gxt.widget.core.client.form.ConverterEditorAdapter;
import com.sencha.gxt.widget.core.client.form.Field;
import com.sencha.gxt.widget.core.client.form.FieldLabel;
import com.sencha.gxt.widget.core.client.form.FieldSet;
import de.mec.client.editor.CheckboxItemEditor;
import de.mec.client.editor.ComboboxItemEditor;
import de.mec.client.editor.ItemEditorFactory;
import de.mec.client.editor.SliderItemEditor;
import de.mec.client.editor.TextItemEditor;
import de.mec.client.editor.guimodel.GUITemplate;
import de.mec.client.editor.guimodel.Group;
import de.mec.client.editor.guimodel.Option;
import de.mec.client.model.Codec;
import de.mec.client.model.CodecProperties;
import de.mec.client.model.tools.CodecIdConverter;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class VideoPropertyListEditor extends Composite implements
        CompositeEditor<List<PropertyItem>, PropertyItem, PropertyItemEditor>,
        ValueAwareEditor<List<PropertyItem>> {

    private List<PropertyItem> _currentData;
    private EditorDelegate<List<PropertyItem>> _delegate;
    EditorChain<PropertyItem, PropertyItemEditor> _chain;
    private static final Logger logger = Logger.getLogger(VideoEditor.class.toString());
    private List<PropertyItemEditor> editors = new ArrayList<PropertyItemEditor>();
    private FlowLayoutContainer vlc;
    private ComboBox<Codec> codecCombo;
    private ConverterEditorAdapter<String, Codec, ComboBox<Codec>> codecId;
    private ListStore<Codec> codecs;
    private HashMap<String, VerticalLayoutContainer> _fieldSets = new HashMap<String, VerticalLayoutContainer>();
    private HashMap<String, PropertyItem> _propertyMap = new HashMap<String, PropertyItem>();

    public VideoPropertyListEditor() {
        vlc = new FlowLayoutContainer();
        vlc.setSize("100%", "400");
        vlc.setScrollMode(ScrollSupport.ScrollMode.AUTO);
        initWidget(vlc);
        CodecProperties cprops = GWT.create(CodecProperties.class);
        MHiveService mhiveService = new MHiveService();
        codecs = new ListStore<Codec>(cprops.id());
        mhiveService.getVideoCodecs(new AsyncCallback<List<Codec>>() {
            public void onFailure(Throwable caught) {
                throw new UnsupportedOperationException("Not supported yet.");
            }

            public void onSuccess(List<Codec> result) {
                //throw new UnsupportedOperationException("Not supported yet.");
                codecs.addAll(result);
                //logger.log(Level.INFO,"CodecId:"+result.size());
            }
        });
        codecCombo = new ComboBox<Codec>(codecs, cprops.longName());
        codecCombo.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
        codecCombo.setForceSelection(true);
        codecCombo.setWidth(400);
        codecCombo.addSelectionHandler(new SelectionHandler<Codec>() {
            public void onSelection(SelectionEvent<Codec> event) {
                logger.log(Level.INFO, "Field value:" + event.getSelectedItem().getId());
                buildUI(event.getSelectedItem().getId());
                if (!_propertyMap.containsKey("id")) {
                    _propertyMap.put("id", new PropertyItem("id", event.getSelectedItem().getId()));
                    _currentData.add(_propertyMap.get("id"));
                } else {
                    _propertyMap.get("id").setVal(event.getSelectedItem().getId());
                }
            }
        });
        FieldSet panel = new FieldSet();
        panel.setHeadingText("select the Codec");
        Grid grid=new Grid(1,2);
        panel.add(grid);
        FieldLabel label=new FieldLabel();
        label.setText("Codec");
        grid.setWidget(0, 0, label);
        grid.setWidget(0, 1, codecCombo);
        vlc.add(panel);

        //codecId = new ConverterEditorAdapter<String, Codec, ComboBox<Codec>>(codecCombo, new CodecIdConverter(codecs));

    }

    public PropertyItemEditor createEditorForTraversal() {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public String getPathElement(PropertyItemEditor subEditor) {
        //logger.log(Level.INFO, "subEditor:" + subEditor);
        return "";//"[" + editors.indexOf(subEditor) + "]";
    }

    public void setEditorChain(EditorChain<PropertyItem, PropertyItemEditor> chain) {
        _chain = chain;
    }

    public void flush() {
        for (PropertyItemEditor editor : editors) {
            PropertyItem entry = _chain.getValue(editor);
            logger.log(Level.INFO, "flush key=" + entry.getKey() + " val:" + entry.getVal());
            //_currentData.put(entry.getKey(), entry.getValue());
        }
    }

    public void onPropertyChange(String... paths) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void setValue(List<PropertyItem> value) {
        _currentData = value;
        for (final PropertyItem entry : value) {
            _propertyMap.put(entry.getKey(), entry);

            //final PropertyItemEditor editor = ItemEditorFactory.getEditor(entry.getKey());

            //editor.setLabel(entry.getKey());
            //editor.setKey(entry.getKey());
            if (entry.getKey().equals("id")) {
                codecCombo.setValue(new CodecIdConverter(codecs).convertModelValue(entry.getVal()));
                /*loading initial codec ui*/
                buildUI(entry.getVal());
                continue;
            }
            //editors.add(editor);
            //_chain.attach(entry, editor);
            //vlc.add(editor);
            /*
             editor.addValueChangeHandler(new ValueChangeHandler() {
             public void onValueChange(ValueChangeEvent event) {
             logger.log(Level.INFO, "Field name:" + editor.getKey());
             logger.log(Level.INFO, "Field value:" + event.getValue());
             }
             });
             */
        }
    }

    public void setDelegate(EditorDelegate<List<PropertyItem>> delegate) {
        _delegate = delegate;
    }

    public List<PropertyItem> getValue() {
        return _currentData;
    }

    private void buildUI(final String codecID) {
        MHiveService mhiveService = new MHiveService();
        mhiveService.getVideoUI(codecID, new AsyncCallback<GUITemplate>() {
            public void onFailure(Throwable caught) {
                //throw new UnsupportedOperationException("Not supported yet.");
            }

            public void onSuccess(GUITemplate result) {
                clearUI();
                buildUI(result);
                //logger.log(Level.INFO,"GUI received"+result.getGroups());
            }
        });
    }

    private void buildUI(final GUITemplate gui) {
        for (Group group : gui.getGroups()) {
            //logger.log(Level.INFO,"Create simple panel");
            FieldSet panel = new FieldSet();
            panel.setHeadingText(group.getTitle());
            VerticalLayoutContainer p = new VerticalLayoutContainer();
            panel.add(p);
            vlc.add(panel);
            _fieldSets.put(group.getId(), p);
        }

        for (Option option : gui.getOptions()) {
            //logger.log(Level.INFO,"Create PropertyItemEditor:"+option.getControl().getType());
            final PropertyItemEditor editor = ItemEditorFactory.getEditorByClassName(option.getControl().getType());
            editor.setLabel(option.getTitle());
            editor.setKey(option.getId());
            if (editor instanceof ComboboxItemEditor) {
                logger.log(Level.INFO, "setting items for combobox" + option.getControl().getDefault());
                ((ComboboxItemEditor) editor).setItems(option.getControl().getItems());
            }
            _fieldSets.get(option.getGroup()).add(editor);

            /*create propertyItem when not exists*/
            if (!_propertyMap.containsKey(option.getId())) {
                _propertyMap.put(option.getId(), new PropertyItem(option.getId(), option.getControl().getDefault()));
                _currentData.add(_propertyMap.get(option.getId()));
            }

            /*attache it to the chain*/
            _chain.attach(_propertyMap.get(option.getId()), editor);


            editor.addValueChangeHandler(new ValueChangeHandler() {
                public void onValueChange(ValueChangeEvent event) {
                    logger.log(Level.INFO, "Field name:" + editor.getKey());
                    logger.log(Level.INFO, "Field value:" + event.getValue());
                }
            });

        }
    }

    private void clearUI() {
        for (Map.Entry<String, VerticalLayoutContainer> entry : _fieldSets.entrySet()) {
            int widget_count = entry.getValue().getWidgetCount();
            for (int a = 0; a < widget_count; a++) {
                PropertyItemEditor editor = (PropertyItemEditor) entry.getValue().getWidget(0);
                /*detach it from the chain*/
                _chain.detach(editor);
                editor.removeFromParent();
            }
            entry.getValue().getParent().removeFromParent();
        }
    }
}
