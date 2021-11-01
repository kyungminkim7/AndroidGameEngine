// automatically generated by the FlatBuffers compiler, do not modify

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Rapunzel extends Struct {
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public Rapunzel __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int hairLength() { return bb.getInt(bb_pos + 0); }
  public void mutateHairLength(int hair_length) { bb.putInt(bb_pos + 0, hair_length); }

  public static int createRapunzel(FlatBufferBuilder builder, int hairLength) {
    builder.prep(4, 4);
    builder.putInt(hairLength);
    return builder.offset();
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public Rapunzel get(int j) { return get(new Rapunzel(), j); }
    public Rapunzel get(Rapunzel obj, int j) {  return obj.__assign(__element(j), bb); }
  }
  public RapunzelT unpack() {
    RapunzelT _o = new RapunzelT();
    unpackTo(_o);
    return _o;
  }
  public void unpackTo(RapunzelT _o) {
    int _oHairLength = hairLength();
    _o.setHairLength(_oHairLength);
  }
  public static int pack(FlatBufferBuilder builder, RapunzelT _o) {
    if (_o == null) return 0;
    return createRapunzel(
      builder,
      _o.getHairLength());
  }
}

