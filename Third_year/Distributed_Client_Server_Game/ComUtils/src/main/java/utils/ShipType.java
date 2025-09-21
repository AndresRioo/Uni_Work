package utils;

/**
 * Enum for the different types of ships.
 */
public enum ShipType {

    /**
     * LONGSHIP (5 cells)
     */
    LONGSHIP((byte)1), // 5 casillas

    /**
     * FRIGATE (4 cells)
     */
    FRIGATE((byte)2), // 4 casillas

    /**
     * BRIG (3 cells)
     */
    BRIG((byte)3), // 3 casillas

    /**
     * SCHOONER (3 cells)
     */
    SCHOONER((byte)4), // 3 casillas

    /**
     * SLOOP (2 cells)
     */
    SLOOP((byte)5); // 2 casillas

    /**
     * The code of the ship type.
     */
    private final byte code;


    ShipType(byte code) {
        this.code = code;
    }

    public byte getCode() {
        return code;
    }

    /**
     * Returns the ShipType from the code provided.
     * @param code Code of the ShipType (1-5)
     * @return Name of ship type
     */
    public static ShipType fromCode(byte code) {
        for (ShipType shipType : ShipType.values()) {
            if (shipType.code == code) {
                return shipType;
            }
        }
        return null;
    }

    /**
     * Returns the size of the ship type.
     * @param type Ship type
     * @return Size of the ship type
     */
    public static int typeToSize(byte type){
        switch (type){
            case 1: // LONGSHIP (5 casillas)
                return 5;
            case 2: // FRIGATE (4 casillas)
                return 4;
            case 3: // BRIG (3 casillas)
                return 3;
            case 4: // SCHOONER (3 casillas)
                return 3;
            case 5: // SLOOP (2 casillas)
                return 2;
            default:
                return -1;
        }
    }
}
