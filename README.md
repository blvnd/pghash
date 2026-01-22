Moduł oblicza partycje w partycjonowaniu postgresa HASH, obsluga int8 - int4 też wygląda że się poprawnie kalkuluje.

(((unsigned2signed(hashint8extended(tripId, 8816678312871386365)) + 5305509591434766563) % 18446744073709551616) % bucketNum)
